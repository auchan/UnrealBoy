// License: See LICENSE.txt file

#include "UnrealBoyAPU.h"
#include "UnrealBoyAudioChannels.h"
#include "UnrealBoyMotherboard.h"
#include "AudioMixerDevice.h"
#include "Engine/Engine.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Engine/World.h"

FUnrealBoyAPU::FUnrealBoyAPU(FUnrealBoyMotherboard& InMotherboard)
	: Motherboard(InMotherboard)
	, bAudioEnabled(false)
	, FrameSequencerCounter(0)
	, FrameSequencerStep(0)
	, LeftOutput(0.0f)
	, RightOutput(0.0f)
	, MasterVolume(1.0f)
	, LeftHPFCapacitor(0.0f)
	, RightHPFCapacitor(0.0f)
	, SampleCounter(0)
	, LeftAccumulator(0.0f)
	, RightAccumulator(0.0f)
	, AccumulatorCount(0)
	, ProceduralSoundWave(nullptr)
	, AudioComponent(nullptr)
	, bAudioPlaybackActive(false)
	, AudioQueueReadIndex(0)
	, TestAudioPhase(0.0f)
	, bGenerateTestAudio(false)
{
	// Initialize channels
	Channel1 = MakeShared<FUnrealBoyChannel1>(Motherboard);
	Channel2 = MakeShared<FUnrealBoyChannel2>(Motherboard);
	Channel3 = MakeShared<FUnrealBoyChannel3>(Motherboard);
	Channel4 = MakeShared<FUnrealBoyChannel4>(Motherboard);

	// Initialize audio buffer
	AudioBuffer.Reserve(SAMPLES_PER_FRAME * 2); // Stereo
	DownsampledBuffer.Reserve(SAMPLES_PER_FRAME * 2); // Stereo
}

FUnrealBoyAPU::~FUnrealBoyAPU()
{
	Shutdown();
}

void FUnrealBoyAPU::Serialize(FArchive& Ar)
{
	Ar << bAudioEnabled;
	Ar << FrameSequencerCounter;
	Ar << FrameSequencerStep;
	Ar << LeftHPFCapacitor;
	Ar << RightHPFCapacitor;
}

void FUnrealBoyAPU::Initialize()
{
	InitializeMemoryHandlers();
	
	// Create procedural sound wave for audio output
	ProceduralSoundWave = NewObject<USoundWaveProcedural>();
	if (ProceduralSoundWave.IsValid())
	{
		// Prevent garbage collection - we're a plain C++ class, not a UObject with UPROPERTY
		ProceduralSoundWave->AddToRoot();

		ProceduralSoundWave->SetSampleRate(SAMPLE_RATE);
		ProceduralSoundWave->NumChannels = 2; // Stereo
		ProceduralSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
		ProceduralSoundWave->SoundGroup = SOUNDGROUP_Default;
		ProceduralSoundWave->bLooping = true;
		// SampleByteSize defaults to 2 (int16 PCM) - we convert float to int16 before queuing
	}

	// Reset APU state
	bAudioEnabled = false;
	FrameSequencerCounter = 0;
	FrameSequencerStep = 0;
	LeftHPFCapacitor = 0.0f;
	RightHPFCapacitor = 0.0f;
	SampleCounter = 0;
	LeftAccumulator = 0.0f;
	RightAccumulator = 0.0f;
	AccumulatorCount = 0;
	DownsampledBuffer.Reset();

	// Start audio playback
	StartAudioPlayback();
}

void FUnrealBoyAPU::Shutdown()
{
	// Stop audio playback
	StopAudioPlayback();

	if (ProceduralSoundWave.IsValid())
	{
		ProceduralSoundWave->RemoveFromRoot();
		ProceduralSoundWave = nullptr;
	}

	if (AudioComponent.IsValid())
	{
		AudioComponent->RemoveFromRoot();
		AudioComponent->DestroyComponent();
		AudioComponent = nullptr;
	}

	// Clear audio queue
	AudioQueue.Empty();
	AudioQueueReadIndex = 0;
}

void FUnrealBoyAPU::Tick()
{
	if (!bAudioEnabled)
	{
		return;
	}

	// Tick all channels
	Channel1->Tick();
	Channel2->Tick();
	Channel3->Tick();
	Channel4->Tick();

	// Frame sequencer runs at 512 Hz (CPU_FREQUENCY / 8192)
	FrameSequencerCounter++;
	if (FrameSequencerCounter >= (CPU_FREQUENCY / 512))
	{
		FrameSequencerCounter = 0;
		TickFrameSequencer();
	}

	// Mix channels every CPU tick and accumulate for box-car averaging (anti-aliasing)
	MixChannels();
	LeftAccumulator += LeftOutput;
	RightAccumulator += RightOutput;
	AccumulatorCount++;

	// Downsample: emit one audio sample every CPU_FREQUENCY/SAMPLE_RATE (~95) CPU cycles
	SampleCounter += SAMPLE_RATE;
	if (SampleCounter >= CPU_FREQUENCY)
	{
		SampleCounter -= CPU_FREQUENCY;

		// Average the accumulated mix values over this sample period (box-car anti-aliasing)
		float AvgLeft = LeftAccumulator / static_cast<float>(AccumulatorCount);
		float AvgRight = RightAccumulator / static_cast<float>(AccumulatorCount);
		LeftAccumulator = 0.0f;
		RightAccumulator = 0.0f;
		AccumulatorCount = 0;

		// Apply DC-blocking high-pass filter at the output sample rate
		float FilteredLeft = ApplyHighPassFilter(AvgLeft, LeftHPFCapacitor);
		float FilteredRight = ApplyHighPassFilter(AvgRight, RightHPFCapacitor);

		// Store stereo sample pair (L, R) in downsampled buffer
		DownsampledBuffer.Add(FilteredLeft);
		DownsampledBuffer.Add(FilteredRight);
	}
}

void FUnrealBoyAPU::GenerateAudioFrame(TArray<float>& OutSamples)
{
	OutSamples.Reset();
	OutSamples.Reserve(SAMPLES_PER_FRAME * 2); // Stereo

	if (!bAudioEnabled)
	{
		// Generate silence
		for (uint32 i = 0; i < SAMPLES_PER_FRAME * 2; i++)
		{
			OutSamples.Add(0.0f);
		}
		return;
	}

	// Note: This function is kept for compatibility but the main audio path
	// now uses DownsampledBuffer populated during Tick() for proper downsampling.
	for (uint32 SampleIndex = 0; SampleIndex < SAMPLES_PER_FRAME; SampleIndex++)
	{
		float FilteredLeft = ApplyHighPassFilter(LeftOutput, LeftHPFCapacitor);
		float FilteredRight = ApplyHighPassFilter(RightOutput, RightHPFCapacitor);

		OutSamples.Add(FilteredLeft);
		OutSamples.Add(FilteredRight);
	}
}

void FUnrealBoyAPU::InitializeMemoryHandlers()
{
	// Register memory write handlers for APU control registers
	// This would need to be integrated with the motherboard's memory system
	// For now, we'll assume the motherboard will call these handlers
}

TMap<uint16, FUnrealBoyWriteMemoryDelegate> FUnrealBoyAPU::GetMemoryWriteDelegates()
{
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Delegates;
	
	// APU control registers
	Delegates.Add(UnrealBoyAddressNames::NR50).BindRaw(this, &FUnrealBoyAPU::OnWriteNR50);
	Delegates.Add(UnrealBoyAddressNames::NR51).BindRaw(this, &FUnrealBoyAPU::OnWriteNR51);
	Delegates.Add(UnrealBoyAddressNames::NR52).BindRaw(this, &FUnrealBoyAPU::OnWriteNR52);
	
	// Channel 1 delegates
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Channel1Delegates = Channel1->GetMemoryWriteDelegates();
	for (const auto& Pair : Channel1Delegates)
	{
		Delegates.Add(Pair.Key, Pair.Value);
	}
	
	// Channel 2 delegates
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Channel2Delegates = Channel2->GetMemoryWriteDelegates();
	for (const auto& Pair : Channel2Delegates)
	{
		Delegates.Add(Pair.Key, Pair.Value);
	}
	
	// Channel 3 delegates
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Channel3Delegates = Channel3->GetMemoryWriteDelegates();
	for (const auto& Pair : Channel3Delegates)
	{
		Delegates.Add(Pair.Key, Pair.Value);
	}
	
	// Channel 4 delegates
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Channel4Delegates = Channel4->GetMemoryWriteDelegates();
	for (const auto& Pair : Channel4Delegates)
	{
		Delegates.Add(Pair.Key, Pair.Value);
	}
	
	return Delegates;
}

bool FUnrealBoyAPU::OnWriteNR50(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	// NR50: Channel control / ON-OFF / Volume
	// Bit 7: VIN->SO2 ON/OFF
	// Bit 6-4: SO2 output level (volume)
	// Bit 3: VIN->SO1 ON/OFF  
	// Bit 2-0: SO1 output level (volume)
	// Volume is applied per-channel in MixChannels() using NR50 directly from memory
	return true;
}

bool FUnrealBoyAPU::OnWriteNR51(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	// NR51: Selection of Sound output terminal
	// Controls which channels go to which output (left/right)
	// This will be handled in the mixing stage
	return true;
}

bool FUnrealBoyAPU::OnWriteNR52(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	// NR52: Sound on/off
	// Bit 7: All sound on/off
	// Bit 3: Sound 4 ON flag
	// Bit 2: Sound 3 ON flag  
	// Bit 1: Sound 2 ON flag
	// Bit 0: Sound 1 ON flag
	
	bool NewAudioEnabled = (NewValue & 0x80) != 0;
	
	if (!NewAudioEnabled && bAudioEnabled)
	{
		// Turning off audio - clear all registers except length counters
		bAudioEnabled = false;
		FrameSequencerCounter = 0;
		FrameSequencerStep = 0;
		StopAudioPlayback();
	}
	else if (NewAudioEnabled && !bAudioEnabled)
	{
		// Turning on audio
		bAudioEnabled = true;
		FrameSequencerCounter = 0;
		FrameSequencerStep = 0;
		StartAudioPlayback();
	}
	
	return true;
}

void FUnrealBoyAPU::TickFrameSequencer()
{
	// Frame sequencer runs at 512 Hz with 8 steps
	switch (FrameSequencerStep)
	{
	case 0: // Length counter
		Channel1->TickLengthCounter();
		Channel2->TickLengthCounter();
		Channel3->TickLengthCounter();
		Channel4->TickLengthCounter();
		break;
		
	case 1: // Nothing
		break;
		
	case 2: // Length counter and sweep
		Channel1->TickLengthCounter();
		Channel2->TickLengthCounter();
		Channel3->TickLengthCounter();
		Channel4->TickLengthCounter();
		Channel1->TickSweep();
		break;
		
	case 3: // Nothing
		break;
		
	case 4: // Length counter
		Channel1->TickLengthCounter();
		Channel2->TickLengthCounter();
		Channel3->TickLengthCounter();
		Channel4->TickLengthCounter();
		break;
		
	case 5: // Nothing
		break;
		
	case 6: // Length counter and sweep
		Channel1->TickLengthCounter();
		Channel2->TickLengthCounter();
		Channel3->TickLengthCounter();
		Channel4->TickLengthCounter();
		Channel1->TickSweep();
		break;
		
	case 7: // Volume envelope
		Channel1->TickVolumeEnvelope();
		Channel2->TickVolumeEnvelope();
		Channel4->TickVolumeEnvelope();
		break;
	}
	
	FrameSequencerStep = (FrameSequencerStep + 1) % 8;
}

void FUnrealBoyAPU::MixChannels()
{
	if (!bAudioEnabled)
	{
		LeftOutput = 0.0f;
		RightOutput = 0.0f;
		return;
	}

	// Get NR51 register for channel routing
	uint8 NR51Value = UNREALBOY_GET_MEMORY_REF(NR51);
	
	float LeftMix = 0.0f;
	float RightMix = 0.0f;
	
	// Channel 1
	if (Channel1->IsEnabled() && Channel1->IsDACEnabled())
	{
		float Sample = (Channel1->GetDigitalOutput() / 15.0f) * 2.0f - 1.0f; // Convert to -1 to 1
		if (NR51Value & 0x10) LeftMix += Sample;   // CH1 -> SO1
		if (NR51Value & 0x01) RightMix += Sample;  // CH1 -> SO2
	}
	
	// Channel 2
	if (Channel2->IsEnabled() && Channel2->IsDACEnabled())
	{
		float Sample = (Channel2->GetDigitalOutput() / 15.0f) * 2.0f - 1.0f;
		if (NR51Value & 0x20) LeftMix += Sample;   // CH2 -> SO1
		if (NR51Value & 0x02) RightMix += Sample;  // CH2 -> SO2
	}
	
	// Channel 3
	if (Channel3->IsEnabled() && Channel3->IsDACEnabled())
	{
		float Sample = (Channel3->GetDigitalOutput() / 15.0f) * 2.0f - 1.0f;
		if (NR51Value & 0x40) LeftMix += Sample;   // CH3 -> SO1
		if (NR51Value & 0x04) RightMix += Sample;  // CH3 -> SO2
	}
	
	// Channel 4
	if (Channel4->IsEnabled() && Channel4->IsDACEnabled())
	{
		float Sample = (Channel4->GetDigitalOutput() / 15.0f) * 2.0f - 1.0f;
		if (NR51Value & 0x80) LeftMix += Sample;   // CH4 -> SO1
		if (NR51Value & 0x08) RightMix += Sample;  // CH4 -> SO2
	}
	
	// Apply master volume from NR50
	uint8 NR50Value = UNREALBOY_GET_MEMORY_REF(NR50);
	float LeftVolume = (NR50Value & 0x07) / 7.0f;
	float RightVolume = ((NR50Value >> 4) & 0x07) / 7.0f;
	
	LeftOutput = LeftMix * LeftVolume * 0.5f; // Scale factor for overall volume (0.25 = accurate, 0.5 = boosted)
	RightOutput = RightMix * RightVolume * 0.5f;
}

float FUnrealBoyAPU::ApplyHighPassFilter(float Input, float& Capacitor)
{
	// DC-blocking high-pass filter (capacitor model)
	// output = input - capacitor
	// capacitor += output * charge_factor
	// charge_factor tuned for 44100 Hz sample rate
	constexpr float ChargeFactor = 0.999958f;
	float Output = Input - Capacitor;
	Capacitor += Output * ChargeFactor;
	return Output;
}

void FUnrealBoyAPU::StartAudioPlayback()
{
	if (bAudioPlaybackActive || !ProceduralSoundWave.IsValid())
	{
		return;
	}

	// Create audio component if it doesn't exist
	if (!AudioComponent.IsValid())
	{
		if (GEngine && GEngine->GetCurrentPlayWorld())
		{
			UWorld* World = GEngine->GetCurrentPlayWorld();
			AudioComponent = NewObject<UAudioComponent>(World);
			if (AudioComponent.IsValid())
			{
				// Prevent garbage collection - we're a plain C++ class, not a UObject with UPROPERTY
				AudioComponent->AddToRoot();

				AudioComponent->SetSound(ProceduralSoundWave.Get());
				AudioComponent->bAutoActivate = false;
				AudioComponent->bIsUISound = true; // Prevent audio from being affected by game pause
				AudioComponent->RegisterComponentWithWorld(World);
			}
		}
	}

	if (AudioComponent.IsValid())
	{
		AudioComponent->Play();
		bAudioPlaybackActive = true;
	}
}

void FUnrealBoyAPU::StopAudioPlayback()
{
	if (AudioComponent.IsValid() && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}
	bAudioPlaybackActive = false;
}

void FUnrealBoyAPU::UpdateAudioOutput()
{
	if (!bAudioPlaybackActive)
	{
		return;
	}

	// Detect and recover from AudioComponent stopping (e.g., buffer underrun)
	if (AudioComponent.IsValid() && !AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
	}

	// If in test mode, generate test audio instead
	if (bGenerateTestAudio)
	{
		GenerateTestAudio();
		return;
	}

	if (!ProceduralSoundWave.IsValid())
	{
		DownsampledBuffer.Reset();
		return;
	}

	// Feed downsampled audio directly to the procedural sound wave
	// These samples were captured at SAMPLE_RATE (44100 Hz) during Tick()
	if (DownsampledBuffer.Num() > 0)
	{
		// Convert float samples to int16 PCM (SampleByteSize=2 is the UE default)
		TArray<int16> PCMBuffer;
		PCMBuffer.SetNumUninitialized(DownsampledBuffer.Num());
		for (int32 i = 0; i < DownsampledBuffer.Num(); ++i)
		{
			PCMBuffer[i] = static_cast<int16>(FMath::Clamp(DownsampledBuffer[i], -1.0f, 1.0f) * 32767.0f);
		}

		ProceduralSoundWave->QueueAudio(
			reinterpret_cast<const uint8*>(PCMBuffer.GetData()),
			PCMBuffer.Num() * sizeof(int16));
		DownsampledBuffer.Reset();
	}
}

void FUnrealBoyAPU::GenerateTestAudio(float Frequency, float Volume)
{
	bGenerateTestAudio = true;
	
	// Generate a simple sine wave for testing
	TArray<float> TestSamples;
	TestSamples.Reserve(SAMPLES_PER_FRAME * 2);
	
	const float PhaseIncrement = (Frequency * 2.0f * PI) / SAMPLE_RATE;
	
	for (uint32 i = 0; i < SAMPLES_PER_FRAME; ++i)
	{
		float Sample = FMath::Sin(TestAudioPhase) * Volume;
		TestSamples.Add(Sample); // Left
		TestSamples.Add(Sample); // Right
		
		TestAudioPhase += PhaseIncrement;
		if (TestAudioPhase > 2.0f * PI)
		{
			TestAudioPhase -= 2.0f * PI;
		}
	}
	
	// Convert float to int16 PCM and queue the test audio
	if (ProceduralSoundWave.IsValid() && bAudioPlaybackActive)
	{
		TArray<int16> PCMTestSamples;
		PCMTestSamples.SetNumUninitialized(TestSamples.Num());
		for (int32 i = 0; i < TestSamples.Num(); ++i)
		{
			PCMTestSamples[i] = static_cast<int16>(FMath::Clamp(TestSamples[i], -1.0f, 1.0f) * 32767.0f);
		}
		ProceduralSoundWave->QueueAudio(reinterpret_cast<const uint8*>(PCMTestSamples.GetData()),
			PCMTestSamples.Num() * sizeof(int16));
	}
}

// Base audio channel implementation
FUnrealBoyAudioChannel::FUnrealBoyAudioChannel(FUnrealBoyMotherboard& InMotherboard)
	: Motherboard(InMotherboard)
	, bChannelEnabled(false)
	, bDACEnabled(false)
{
}

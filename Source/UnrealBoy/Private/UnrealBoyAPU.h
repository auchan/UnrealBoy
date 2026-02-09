// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"
#include "AudioMixerDevice.h"
#include "AudioMixerSubmix.h"
#include "Sound/SoundWaveProcedural.h"

// Forward declarations
class FUnrealBoyMotherboard;
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FUnrealBoyWriteMemoryDelegate, uint16 /** Address */, uint8 /** OldValue */, uint8 /** NewValue */);

/**
 * GameBoy Audio Processing Unit (APU)
 * Implements the 4-channel audio system of the original GameBoy
 */
class UNREALBOY_API FUnrealBoyAPU : public IUnrealBoySerializable
{
public:
	FUnrealBoyAPU(FUnrealBoyMotherboard& InMotherboard);
	virtual ~FUnrealBoyAPU() override;

	virtual void Serialize(FArchive& Ar) override;

	/** Initialize the audio system */
	void Initialize();

	/** Shutdown the audio system */
	void Shutdown();

	/** Tick the APU - should be called at 4194304 Hz (CPU clock rate) */
	void Tick();

	/** Generate audio samples for the current frame */
	void GenerateAudioFrame(TArray<float>& OutSamples);

	/** Check if audio is enabled */
	bool IsAudioEnabled() const { return bAudioEnabled; }

	/** Get memory write delegates for motherboard registration */
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> GetMemoryWriteDelegates();

	/** Start audio playback */
	void StartAudioPlayback();

	/** Stop audio playback */
	void StopAudioPlayback();

	/** Update audio output (should be called every frame) */
	void UpdateAudioOutput();

	/** Generate test audio (for debugging) */
	void GenerateTestAudio(float Frequency = 440.0f, float Volume = 0.3f);

	/** Apply DC-blocking high-pass filter (capacitor model) */
	float ApplyHighPassFilter(float Input, float& Capacitor);

	/** Mix all channels into stereo output */
	void MixChannels();

	/** Frame sequencer tick (called at 512 Hz) */
	void TickFrameSequencer();

private:
	/** Reference to the motherboard for memory access */
	FUnrealBoyMotherboard& Motherboard;

	/** Audio enabled flag */
	bool bAudioEnabled;

	/** Frame sequencer counter (512 Hz) */
	uint32 FrameSequencerCounter;
	uint32 FrameSequencerStep;

	/** Sample rate and buffer management */
	static constexpr uint32 SAMPLE_RATE = 44100;
	static constexpr uint32 CPU_FREQUENCY = 4194304;
	static constexpr uint32 SAMPLES_PER_FRAME = SAMPLE_RATE / 60; // ~735 samples per frame at 60 FPS

	/** Audio channels */
	TSharedPtr<class FUnrealBoyChannel1> Channel1; // Square wave with sweep
	TSharedPtr<class FUnrealBoyChannel2> Channel2; // Square wave
	TSharedPtr<class FUnrealBoyChannel3> Channel3; // Wave channel
	TSharedPtr<class FUnrealBoyChannel4> Channel4; // Noise channel

	/** Mixer and output */
	float LeftOutput;
	float RightOutput;
	float MasterVolume;

	/** High-pass filter state (DC-blocking capacitor model) */
	float LeftHPFCapacitor;
	float RightHPFCapacitor;

	/** Downsampling: emits samples at SAMPLE_RATE during Tick() */
	uint32 SampleCounter;
	TArray<float> DownsampledBuffer;

	/** Box-car averaging accumulators for anti-aliased downsampling */
	float LeftAccumulator;
	float RightAccumulator;
	uint32 AccumulatorCount;

	/** Audio buffer for Unreal Engine */
	TArray<float> AudioBuffer;
	TWeakObjectPtr<USoundWaveProcedural> ProceduralSoundWave;

	/** Audio playback components */
	TWeakObjectPtr<UAudioComponent> AudioComponent;
	bool bAudioPlaybackActive;
	
	/** Audio streaming buffer */
	TArray<float> AudioQueue;
	uint32 AudioQueueReadIndex;
	static constexpr uint32 AUDIO_BUFFER_SIZE = SAMPLES_PER_FRAME * 4; // Buffer multiple frames

	/** Test audio generation */
	float TestAudioPhase;
	bool bGenerateTestAudio;

	/** Memory register handlers */
	void InitializeMemoryHandlers();
	bool OnWriteNR50(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR51(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR52(uint16 Address, uint8 OldValue, uint8 NewValue);
}; 
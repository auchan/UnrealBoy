// License: See LICENSE.txt file

#include "UnrealBoyAudioChannels.h"
#include "UnrealBoyMotherboard.h"

// Duty cycle patterns for square wave channels
static const uint8 DutyPatterns[4][8] = {
	{0, 0, 0, 0, 0, 0, 0, 1}, // 12.5%
	{1, 0, 0, 0, 0, 0, 0, 1}, // 25%
	{1, 0, 0, 0, 0, 1, 1, 1}, // 50%
	{0, 1, 1, 1, 1, 1, 1, 0}, // 75%
};

//=============================================================================
// Channel 1: Square wave with frequency sweep
//=============================================================================

FUnrealBoyChannel1::FUnrealBoyChannel1(FUnrealBoyMotherboard& InMotherboard)
	: FUnrealBoyAudioChannel(InMotherboard)
	, SweepPace(0)
	, SweepIndividualStep(0)
	, bSweepDirectionSubtraction(false)
	, bSweepEnabled(false)
	, SweepPaceTimer(0)
	, SweepShadowPeriodValue(0)
	, DutyCycle(0)
	, PeriodValue(0)
	, WaveGenerationTimer(0)
	, WaveGenerationTargetTicks(0)
	, DutyPosition(0)
	, LengthCounter(0)
	, bLengthEnabled(false)
	, InitialVolume(0)
	, CurrentVolume(0)
	, bEnvelopeIncrease(false)
	, EnvelopePeriod(0)
	, EnvelopeTimer(0)
{
	InitializeMemoryHandlers();
}

void FUnrealBoyChannel1::Tick()
{
	if (!bChannelEnabled)
		return;

	// Wave generation timer (derived from PeriodValue)
	// Counts up to WaveGenerationTargetTicks
	WaveGenerationTimer++;

	if (WaveGenerationTimer >= WaveGenerationTargetTicks)
	{
		WaveGenerationTimer = 0; // Reset for next cycle
		// Recalculate target ticks based on current PeriodValue, as it might have changed
		// This makes period changes effective after the current sample ends.
		WaveGenerationTargetTicks = (2048 - PeriodValue) * 4;
		DutyPosition = (DutyPosition + 1) % 8;
	}
}

uint8 FUnrealBoyChannel1::GetDigitalOutput()
{
	if (!bChannelEnabled || !bDACEnabled || LengthCounter == 0)
		return 0;

	// Check duty cycle
	if (DutyPatterns[DutyCycle][DutyPosition] == 0)
		return 0;

	return CurrentVolume;
}

bool FUnrealBoyChannel1::IsEnabled() const
{
	return bChannelEnabled && LengthCounter > 0;
}

bool FUnrealBoyChannel1::IsDACEnabled() const
{
	return bDACEnabled;
}

void FUnrealBoyChannel1::Trigger()
{
	bChannelEnabled = true;
	
	// Length counter
	if (LengthCounter == 0)
		LengthCounter = 64;
	
	// Wave generation timer (derived from PeriodValue)
	// Initialize for up-counting
	WaveGenerationTimer = 0;
	WaveGenerationTargetTicks = (2048 - PeriodValue) * 4;
	// DutyPosition is NOT reset on trigger according to some docs/tests.
	// However, if wave generation was disabled, it might effectively start from 0.
	// For now, let's keep DutyPosition as is, unless specific behavior dictates a reset.
	
	// Volume envelope
	CurrentVolume = InitialVolume;
	EnvelopeTimer = EnvelopePeriod;
	
	// Sweep
	SweepShadowPeriodValue = PeriodValue;
	SweepPaceTimer = SweepPace ? SweepPace : 8; // If SweepPace is 0, timer is 8 (as per docs for sweep timer)
	bSweepEnabled = (SweepPace != 0 || SweepIndividualStep != 0);
	
	// Perform initial sweep calculation and overflow check if sweep step is non-zero
	if (SweepIndividualStep > 0)
	{
		CalculateNewSweepPeriodAndCheckOverflow(); // This first calculation does not write back to PeriodValue or NR13/14
	}
	
	// DAC enabled if volume envelope has non-zero initial volume or increase flag
	bDACEnabled = (InitialVolume != 0 || bEnvelopeIncrease);
}

void FUnrealBoyChannel1::TickLengthCounter()
{
	if (bLengthEnabled && LengthCounter > 0)
	{
		LengthCounter--;
		if (LengthCounter == 0)
		{
			bChannelEnabled = false;
		}
	}
}

void FUnrealBoyChannel1::TickVolumeEnvelope()
{
	if (EnvelopePeriod == 0)
		return;
		
	if (EnvelopeTimer > 0)
	{
		EnvelopeTimer--;
	}
	else
	{
		EnvelopeTimer = EnvelopePeriod;
		
		if (bEnvelopeIncrease && CurrentVolume < 15)
		{
			CurrentVolume++;
		}
		else if (!bEnvelopeIncrease && CurrentVolume > 0)
		{
			CurrentVolume--;
		}
	}
}

void FUnrealBoyChannel1::TickSweep()
{
	if (!bSweepEnabled)
		return;
		
	if (SweepPaceTimer > 0)
	{
		SweepPaceTimer--;
	}
	else
	{
		SweepPaceTimer = SweepPace ? SweepPace : 8; // Reset timer
		
		// Only proceed if sweep pace is non-zero (as per docs: "if the "enabled flag" is set and the sweep pace is not zero")
		if (SweepPace > 0)
		{
			uint16 NewPeriod = CalculateNewSweepPeriodAndCheckOverflow();
			// If new period is valid and sweep shift is non-zero, update registers and shadow
			if (NewPeriod <= 2047 && SweepIndividualStep > 0)
			{
				SweepShadowPeriodValue = NewPeriod;
				PeriodValue = NewPeriod;
				
				// Write back to NR13/NR14
				UNREALBOY_GET_MEMORY_REF(NR13) = PeriodValue & 0xFF;
				UNREALBOY_GET_MEMORY_REF(NR14) = (UNREALBOY_GET_MEMORY_REF(NR14) & 0xF8) | ((PeriodValue >> 8) & 0x07);
				
				// Run frequency calculation and overflow check again immediately, but this second new frequency is not written back
				CalculateNewSweepPeriodAndCheckOverflow();
			}
		}
	}
}

uint16 FUnrealBoyChannel1::CalculateNewSweepPeriodAndCheckOverflow()
{
	uint16 Term = SweepShadowPeriodValue >> SweepIndividualStep;
	uint16 NewPeriod;
	
	if (bSweepDirectionSubtraction) // True for subtraction
	{
		NewPeriod = SweepShadowPeriodValue - Term;
	}
	else // Addition
	{
		NewPeriod = SweepShadowPeriodValue + Term;
	}
	
	// Check for overflow
	if (NewPeriod > 2047)
	{
		bChannelEnabled = false;
	}
	
	return NewPeriod;
}

void FUnrealBoyChannel1::InitializeMemoryHandlers()
{
	// These would be registered with the motherboard's memory system
}

TMap<uint16, FUnrealBoyWriteMemoryDelegate> FUnrealBoyChannel1::GetMemoryWriteDelegates()
{
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Delegates;
	
	Delegates.Add(UnrealBoyAddressNames::NR10).BindRaw(this, &FUnrealBoyChannel1::OnWriteNR10);
	Delegates.Add(UnrealBoyAddressNames::NR11).BindRaw(this, &FUnrealBoyChannel1::OnWriteNR11);
	Delegates.Add(UnrealBoyAddressNames::NR12).BindRaw(this, &FUnrealBoyChannel1::OnWriteNR12);
	Delegates.Add(UnrealBoyAddressNames::NR13).BindRaw(this, &FUnrealBoyChannel1::OnWriteNR13);
	Delegates.Add(UnrealBoyAddressNames::NR14).BindRaw(this, &FUnrealBoyChannel1::OnWriteNR14);
	
	return Delegates;
}

bool FUnrealBoyChannel1::OnWriteNR10(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	SweepPace = (NewValue >> 4) & 0x07;
	bSweepDirectionSubtraction = (NewValue & 0x08) != 0;
	SweepIndividualStep = NewValue & 0x07;
	return true;
}

bool FUnrealBoyChannel1::OnWriteNR11(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	DutyCycle = (NewValue >> 6) & 0x03;
	LengthCounter = 64 - (NewValue & 0x3F);
	return true;
}

bool FUnrealBoyChannel1::OnWriteNR12(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	InitialVolume = (NewValue >> 4) & 0x0F;
	bEnvelopeIncrease = (NewValue & 0x08) != 0;
	EnvelopePeriod = NewValue & 0x07;
	
	// DAC enabled if top 5 bits are not zero
	bDACEnabled = (NewValue & 0xF8) != 0;
	if (!bDACEnabled)
	{
		bChannelEnabled = false;
	}
	
	return true;
}

bool FUnrealBoyChannel1::OnWriteNR13(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	PeriodValue = (PeriodValue & 0x700) | NewValue;
	return true;
}

bool FUnrealBoyChannel1::OnWriteNR14(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	PeriodValue = (PeriodValue & 0xFF) | ((NewValue & 0x07) << 8);
	bLengthEnabled = (NewValue & 0x40) != 0;
	
	if (NewValue & 0x80) // Trigger
	{
		Trigger();
	}
	
	return true;
}

//=============================================================================
// Channel 2: Square wave
//=============================================================================

FUnrealBoyChannel2::FUnrealBoyChannel2(FUnrealBoyMotherboard& InMotherboard)
	: FUnrealBoyAudioChannel(InMotherboard)
	, DutyCycle(0)
	, Frequency(0)
	, FrequencyTimer(0)
	, DutyPosition(0)
	, LengthCounter(0)
	, bLengthEnabled(false)
	, InitialVolume(0)
	, CurrentVolume(0)
	, bEnvelopeIncrease(false)
	, EnvelopePeriod(0)
	, EnvelopeTimer(0)
{
	InitializeMemoryHandlers();
}

void FUnrealBoyChannel2::Tick()
{
	if (!bChannelEnabled)
		return;

	// Frequency timer
	if (FrequencyTimer > 0)
	{
		FrequencyTimer--;
	}
	else
	{
		FrequencyTimer = (2048 - Frequency) * 4;
		DutyPosition = (DutyPosition + 1) % 8;
	}
}

uint8 FUnrealBoyChannel2::GetDigitalOutput()
{
	if (!bChannelEnabled || !bDACEnabled || LengthCounter == 0)
		return 0;

	// Check duty cycle
	if (DutyPatterns[DutyCycle][DutyPosition] == 0)
		return 0;

	return CurrentVolume;
}

bool FUnrealBoyChannel2::IsEnabled() const
{
	return bChannelEnabled && LengthCounter > 0;
}

bool FUnrealBoyChannel2::IsDACEnabled() const
{
	return bDACEnabled;
}

void FUnrealBoyChannel2::Trigger()
{
	bChannelEnabled = true;
	
	// Length counter
	if (LengthCounter == 0)
		LengthCounter = 64;
	
	// Frequency timer
	FrequencyTimer = (2048 - Frequency) * 4;
	
	// Volume envelope
	CurrentVolume = InitialVolume;
	EnvelopeTimer = EnvelopePeriod;
	
	// DAC enabled if volume envelope has non-zero initial volume or increase flag
	bDACEnabled = (InitialVolume != 0 || bEnvelopeIncrease);
}

void FUnrealBoyChannel2::TickLengthCounter()
{
	if (bLengthEnabled && LengthCounter > 0)
	{
		LengthCounter--;
		if (LengthCounter == 0)
		{
			bChannelEnabled = false;
		}
	}
}

void FUnrealBoyChannel2::TickVolumeEnvelope()
{
	if (EnvelopePeriod == 0)
		return;
		
	if (EnvelopeTimer > 0)
	{
		EnvelopeTimer--;
	}
	else
	{
		EnvelopeTimer = EnvelopePeriod;
		
		if (bEnvelopeIncrease && CurrentVolume < 15)
		{
			CurrentVolume++;
		}
		else if (!bEnvelopeIncrease && CurrentVolume > 0)
		{
			CurrentVolume--;
		}
	}
}

void FUnrealBoyChannel2::InitializeMemoryHandlers()
{
	// These would be registered with the motherboard's memory system
}

TMap<uint16, FUnrealBoyWriteMemoryDelegate> FUnrealBoyChannel2::GetMemoryWriteDelegates()
{
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Delegates;
	
	Delegates.Add(UnrealBoyAddressNames::NR21).BindRaw(this, &FUnrealBoyChannel2::OnWriteNR21);
	Delegates.Add(UnrealBoyAddressNames::NR22).BindRaw(this, &FUnrealBoyChannel2::OnWriteNR22);
	Delegates.Add(UnrealBoyAddressNames::NR23).BindRaw(this, &FUnrealBoyChannel2::OnWriteNR23);
	Delegates.Add(UnrealBoyAddressNames::NR24).BindRaw(this, &FUnrealBoyChannel2::OnWriteNR24);
	
	return Delegates;
}

bool FUnrealBoyChannel2::OnWriteNR21(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	DutyCycle = (NewValue >> 6) & 0x03;
	LengthCounter = 64 - (NewValue & 0x3F);
	return true;
}

bool FUnrealBoyChannel2::OnWriteNR22(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	InitialVolume = (NewValue >> 4) & 0x0F;
	bEnvelopeIncrease = (NewValue & 0x08) != 0;
	EnvelopePeriod = NewValue & 0x07;
	
	// DAC enabled if top 5 bits are not zero
	bDACEnabled = (NewValue & 0xF8) != 0;
	if (!bDACEnabled)
	{
		bChannelEnabled = false;
	}
	
	return true;
}

bool FUnrealBoyChannel2::OnWriteNR23(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	Frequency = (Frequency & 0x700) | NewValue;
	return true;
}

bool FUnrealBoyChannel2::OnWriteNR24(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	Frequency = (Frequency & 0xFF) | ((NewValue & 0x07) << 8);
	bLengthEnabled = (NewValue & 0x40) != 0;
	
	if (NewValue & 0x80) // Trigger
	{
		Trigger();
	}
	
	return true;
}

//=============================================================================
// Channel 3: Wave channel
//=============================================================================

FUnrealBoyChannel3::FUnrealBoyChannel3(FUnrealBoyMotherboard& InMotherboard)
	: FUnrealBoyAudioChannel(InMotherboard)
	, WavePosition(0)
	, WaveSample(0)
	, Frequency(0)
	, FrequencyTimer(0)
	, LengthCounter(0)
	, bLengthEnabled(false)
	, VolumeShift(0)
{
	FMemory::Memzero(WaveRAM, sizeof(WaveRAM));
	InitializeMemoryHandlers();
}

void FUnrealBoyChannel3::Tick()
{
	if (!bChannelEnabled || !bDACEnabled)
		return;

	// Frequency timer
	if (FrequencyTimer > 0)
	{
		FrequencyTimer--;
	}
	else
	{
		FrequencyTimer = (2048 - Frequency) * 2;
		WavePosition = (WavePosition + 1) % 32;
		
		// Update wave sample
		uint8 ByteIndex = WavePosition / 2;
		uint8 NibbleIndex = WavePosition % 2;
		
		if (NibbleIndex == 0)
		{
			WaveSample = (WaveRAM[ByteIndex] >> 4) & 0x0F;
		}
		else
		{
			WaveSample = WaveRAM[ByteIndex] & 0x0F;
		}
	}
}

uint8 FUnrealBoyChannel3::GetDigitalOutput()
{
	if (!bChannelEnabled || !bDACEnabled || LengthCounter == 0)
		return 0;

	if (VolumeShift == 0)
		return 0; // Muted
	
	return WaveSample >> (VolumeShift - 1);
}

bool FUnrealBoyChannel3::IsEnabled() const
{
	return bChannelEnabled && LengthCounter > 0;
}

bool FUnrealBoyChannel3::IsDACEnabled() const
{
	return bDACEnabled;
}

void FUnrealBoyChannel3::Trigger()
{
	bChannelEnabled = true;
	
	// Length counter
	if (LengthCounter == 0)
		LengthCounter = 256;
	
	// Frequency timer
	FrequencyTimer = (2048 - Frequency) * 2;
	
	// Reset wave position
	WavePosition = 0;
	
	// Update wave RAM from memory
	UpdateWaveRAM();
}

void FUnrealBoyChannel3::TickLengthCounter()
{
	if (bLengthEnabled && LengthCounter > 0)
	{
		LengthCounter--;
		if (LengthCounter == 0)
		{
			bChannelEnabled = false;
		}
	}
}

void FUnrealBoyChannel3::UpdateWaveRAM()
{
	// Copy wave RAM from memory
	for (int i = 0; i < 16; i++)
	{
		WaveRAM[i] = UNREALBOY_GET_MEMORY_REF(WAVE_RAM_BEGIN + i);
	}
}

void FUnrealBoyChannel3::InitializeMemoryHandlers()
{
	// These would be registered with the motherboard's memory system
}

TMap<uint16, FUnrealBoyWriteMemoryDelegate> FUnrealBoyChannel3::GetMemoryWriteDelegates()
{
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Delegates;
	
	Delegates.Add(UnrealBoyAddressNames::NR30).BindRaw(this, &FUnrealBoyChannel3::OnWriteNR30);
	Delegates.Add(UnrealBoyAddressNames::NR31).BindRaw(this, &FUnrealBoyChannel3::OnWriteNR31);
	Delegates.Add(UnrealBoyAddressNames::NR32).BindRaw(this, &FUnrealBoyChannel3::OnWriteNR32);
	Delegates.Add(UnrealBoyAddressNames::NR33).BindRaw(this, &FUnrealBoyChannel3::OnWriteNR33);
	Delegates.Add(UnrealBoyAddressNames::NR34).BindRaw(this, &FUnrealBoyChannel3::OnWriteNR34);
	
	// Wave RAM
	for (uint16 Address = UnrealBoyAddressNames::WAVE_RAM_BEGIN; Address < UnrealBoyAddressNames::WAVE_RAM_END; Address++)
	{
		Delegates.Add(Address).BindRaw(this, &FUnrealBoyChannel3::OnWriteWaveRAM);
	}
	
	return Delegates;
}

bool FUnrealBoyChannel3::OnWriteNR30(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	bDACEnabled = (NewValue & 0x80) != 0;
	if (!bDACEnabled)
	{
		bChannelEnabled = false;
	}
	return true;
}

bool FUnrealBoyChannel3::OnWriteNR31(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	LengthCounter = 256 - NewValue;
	return true;
}

bool FUnrealBoyChannel3::OnWriteNR32(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	VolumeShift = (NewValue >> 5) & 0x03;
	return true;
}

bool FUnrealBoyChannel3::OnWriteNR33(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	Frequency = (Frequency & 0x700) | NewValue;
	return true;
}

bool FUnrealBoyChannel3::OnWriteNR34(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	Frequency = (Frequency & 0xFF) | ((NewValue & 0x07) << 8);
	bLengthEnabled = (NewValue & 0x40) != 0;
	
	if (NewValue & 0x80) // Trigger
	{
		Trigger();
	}
	
	return true;
}

bool FUnrealBoyChannel3::OnWriteWaveRAM(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	// Update local wave RAM copy
	uint16 Index = Address - UnrealBoyAddressNames::WAVE_RAM_BEGIN;
	if (Index < 16)
	{
		WaveRAM[Index] = NewValue;
	}
	return true;
}

//=============================================================================
// Channel 4: Noise channel
//=============================================================================

FUnrealBoyChannel4::FUnrealBoyChannel4(FUnrealBoyMotherboard& InMotherboard)
	: FUnrealBoyAudioChannel(InMotherboard)
	, LFSR(0x7FFF)
	, bWidthMode(false)
	, ClockShift(0)
	, DivisorCode(0)
	, FrequencyTimer(0)
	, LengthCounter(0)
	, bLengthEnabled(false)
	, InitialVolume(0)
	, CurrentVolume(0)
	, bEnvelopeIncrease(false)
	, EnvelopePeriod(0)
	, EnvelopeTimer(0)
{
	InitializeMemoryHandlers();
}

void FUnrealBoyChannel4::Tick()
{
	if (!bChannelEnabled)
		return;

	// Frequency timer
	if (FrequencyTimer > 0)
	{
		FrequencyTimer--;
	}
	else
	{
		FrequencyTimer = GetDivisor() << ClockShift;
		UpdateLFSR();
	}
}

uint8 FUnrealBoyChannel4::GetDigitalOutput()
{
	if (!bChannelEnabled || !bDACEnabled || LengthCounter == 0)
		return 0;

	// Output is inverted bit 0 of LFSR
	if ((LFSR & 1) == 0)
		return CurrentVolume;
	else
		return 0;
}

bool FUnrealBoyChannel4::IsEnabled() const
{
	return bChannelEnabled && LengthCounter > 0;
}

bool FUnrealBoyChannel4::IsDACEnabled() const
{
	return bDACEnabled;
}

void FUnrealBoyChannel4::Trigger()
{
	bChannelEnabled = true;
	
	// Length counter
	if (LengthCounter == 0)
		LengthCounter = 64;
	
	// Frequency timer
	FrequencyTimer = GetDivisor() << ClockShift;
	
	// Volume envelope
	CurrentVolume = InitialVolume;
	EnvelopeTimer = EnvelopePeriod;
	
	// Reset LFSR
	LFSR = 0x7FFF;
	
	// DAC enabled if volume envelope has non-zero initial volume or increase flag
	bDACEnabled = (InitialVolume != 0 || bEnvelopeIncrease);
}

void FUnrealBoyChannel4::TickLengthCounter()
{
	if (bLengthEnabled && LengthCounter > 0)
	{
		LengthCounter--;
		if (LengthCounter == 0)
		{
			bChannelEnabled = false;
		}
	}
}

void FUnrealBoyChannel4::TickVolumeEnvelope()
{
	if (EnvelopePeriod == 0)
		return;
		
	if (EnvelopeTimer > 0)
	{
		EnvelopeTimer--;
	}
	else
	{
		EnvelopeTimer = EnvelopePeriod;
		
		if (bEnvelopeIncrease && CurrentVolume < 15)
		{
			CurrentVolume++;
		}
		else if (!bEnvelopeIncrease && CurrentVolume > 0)
		{
			CurrentVolume--;
		}
	}
}

uint16 FUnrealBoyChannel4::GetDivisor() const
{
	static const uint16 Divisors[8] = {8, 16, 32, 48, 64, 80, 96, 112};
	return Divisors[DivisorCode];
}

void FUnrealBoyChannel4::UpdateLFSR()
{
	uint8 Bit = (LFSR & 1) ^ ((LFSR >> 1) & 1);
	LFSR >>= 1;
	LFSR |= (Bit << 14);
	
	if (bWidthMode)
	{
		LFSR &= ~(1 << 6);
		LFSR |= (Bit << 6);
	}
}

void FUnrealBoyChannel4::InitializeMemoryHandlers()
{
	// These would be registered with the motherboard's memory system
}

TMap<uint16, FUnrealBoyWriteMemoryDelegate> FUnrealBoyChannel4::GetMemoryWriteDelegates()
{
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> Delegates;
	
	Delegates.Add(UnrealBoyAddressNames::NR41).BindRaw(this, &FUnrealBoyChannel4::OnWriteNR41);
	Delegates.Add(UnrealBoyAddressNames::NR42).BindRaw(this, &FUnrealBoyChannel4::OnWriteNR42);
	Delegates.Add(UnrealBoyAddressNames::NR43).BindRaw(this, &FUnrealBoyChannel4::OnWriteNR43);
	Delegates.Add(UnrealBoyAddressNames::NR44).BindRaw(this, &FUnrealBoyChannel4::OnWriteNR44);
	
	return Delegates;
}

bool FUnrealBoyChannel4::OnWriteNR41(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	LengthCounter = 64 - (NewValue & 0x3F);
	return true;
}

bool FUnrealBoyChannel4::OnWriteNR42(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	InitialVolume = (NewValue >> 4) & 0x0F;
	bEnvelopeIncrease = (NewValue & 0x08) != 0;
	EnvelopePeriod = NewValue & 0x07;
	
	// DAC enabled if top 5 bits are not zero
	bDACEnabled = (NewValue & 0xF8) != 0;
	if (!bDACEnabled)
	{
		bChannelEnabled = false;
	}
	
	return true;
}

bool FUnrealBoyChannel4::OnWriteNR43(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	ClockShift = (NewValue >> 4) & 0x0F;
	bWidthMode = (NewValue & 0x08) != 0;
	DivisorCode = NewValue & 0x07;
	return true;
}

bool FUnrealBoyChannel4::OnWriteNR44(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	bLengthEnabled = (NewValue & 0x40) != 0;
	
	if (NewValue & 0x80) // Trigger
	{
		Trigger();
	}
	
	return true;
} 
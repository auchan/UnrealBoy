// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

// Forward declarations
class FUnrealBoyMotherboard;
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FUnrealBoyWriteMemoryDelegate, uint16 /** Address */, uint8 /** OldValue */, uint8 /** NewValue */);

/**
 * Base class for all audio channels
 */
class UNREALBOY_API FUnrealBoyAudioChannel
{
public:
	FUnrealBoyAudioChannel(FUnrealBoyMotherboard& InMotherboard);
	virtual ~FUnrealBoyAudioChannel() = default;

	/** Tick the channel */
	virtual void Tick() = 0;

	/** Get the current digital output (0-15) */
	virtual uint8 GetDigitalOutput() = 0;

	/** Check if the channel is enabled */
	virtual bool IsEnabled() const = 0;

	/** Check if DAC is enabled */
	virtual bool IsDACEnabled() const = 0;

	/** Trigger the channel */
	virtual void Trigger() = 0;

	/** Frame sequencer steps */
	virtual void TickLengthCounter() {}
	virtual void TickVolumeEnvelope() {}
	virtual void TickSweep() {}

protected:
	FUnrealBoyMotherboard& Motherboard;
	bool bChannelEnabled;
	bool bDACEnabled;
};

/**
 * Channel 1: Square wave with frequency sweep
 */
class UNREALBOY_API FUnrealBoyChannel1 : public FUnrealBoyAudioChannel
{
public:
	FUnrealBoyChannel1(FUnrealBoyMotherboard& InMotherboard);
	virtual ~FUnrealBoyChannel1() override = default;

	virtual void Tick() override;
	virtual uint8 GetDigitalOutput() override;
	virtual bool IsEnabled() const override;
	virtual bool IsDACEnabled() const override;
	virtual void Trigger() override;
	virtual void TickLengthCounter() override;
	virtual void TickVolumeEnvelope() override;
	virtual void TickSweep() override;

	/** Get memory write delegates for motherboard registration */
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> GetMemoryWriteDelegates();

private:
	// Sweep
	uint8 SweepPace;
	uint8 SweepIndividualStep;
	bool bSweepDirectionSubtraction; // True if subtraction (1), false if addition (0)
	bool bSweepEnabled;
	uint8 SweepPaceTimer;
	uint16 SweepShadowPeriodValue;

	// Duty and period (formerly frequency)
	uint8 DutyCycle;
	uint16 PeriodValue; // 11-bit period value from NR13/NR14
	uint16 WaveGenerationTimer; // Timer for wave generation steps, based on PeriodValue. Will now be an up-counter.
	uint16 WaveGenerationTargetTicks; // Target for WaveGenerationTimer for the current cycle
	uint8 DutyPosition;

	// Length counter
	uint8 LengthCounter;
	bool bLengthEnabled;

	// Volume envelope
	uint8 InitialVolume;
	uint8 CurrentVolume;
	bool bEnvelopeIncrease;
	uint8 EnvelopePeriod;
	uint8 EnvelopeTimer;

	// Memory handlers
	void InitializeMemoryHandlers();
	bool OnWriteNR10(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR11(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR12(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR13(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR14(uint16 Address, uint8 OldValue, uint8 NewValue);

	// Helper functions
	uint16 CalculateNewSweepPeriodAndCheckOverflow();
	void UpdateSweep();
};

/**
 * Channel 2: Square wave
 */
class UNREALBOY_API FUnrealBoyChannel2 : public FUnrealBoyAudioChannel
{
public:
	FUnrealBoyChannel2(FUnrealBoyMotherboard& InMotherboard);
	virtual ~FUnrealBoyChannel2() override = default;

	virtual void Tick() override;
	virtual uint8 GetDigitalOutput() override;
	virtual bool IsEnabled() const override;
	virtual bool IsDACEnabled() const override;
	virtual void Trigger() override;
	virtual void TickLengthCounter() override;
	virtual void TickVolumeEnvelope() override;

	/** Get memory write delegates for motherboard registration */
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> GetMemoryWriteDelegates();

private:
	// Duty and frequency
	uint8 DutyCycle;
	uint16 Frequency;
	uint16 FrequencyTimer;
	uint8 DutyPosition;

	// Length counter
	uint8 LengthCounter;
	bool bLengthEnabled;

	// Volume envelope
	uint8 InitialVolume;
	uint8 CurrentVolume;
	bool bEnvelopeIncrease;
	uint8 EnvelopePeriod;
	uint8 EnvelopeTimer;

	// Memory handlers
	void InitializeMemoryHandlers();
	bool OnWriteNR21(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR22(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR23(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR24(uint16 Address, uint8 OldValue, uint8 NewValue);
};

/**
 * Channel 3: Wave channel
 */
class UNREALBOY_API FUnrealBoyChannel3 : public FUnrealBoyAudioChannel
{
public:
	FUnrealBoyChannel3(FUnrealBoyMotherboard& InMotherboard);
	virtual ~FUnrealBoyChannel3() override = default;

	virtual void Tick() override;
	virtual uint8 GetDigitalOutput() override;
	virtual bool IsEnabled() const override;
	virtual bool IsDACEnabled() const override;
	virtual void Trigger() override;
	virtual void TickLengthCounter() override;

	/** Get memory write delegates for motherboard registration */
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> GetMemoryWriteDelegates();

private:
	// Wave data
	uint8 WaveRAM[16];
	uint8 WavePosition;
	uint8 WaveSample;

	// Frequency
	uint16 Frequency;
	uint16 FrequencyTimer;

	// Length counter
	uint16 LengthCounter;
	bool bLengthEnabled;

	// Volume
	uint8 VolumeShift;

	// Memory handlers
	void InitializeMemoryHandlers();
	bool OnWriteNR30(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR31(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR32(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR33(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR34(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteWaveRAM(uint16 Address, uint8 OldValue, uint8 NewValue);

	// Helper functions
	void UpdateWaveRAM();
};

/**
 * Channel 4: Noise channel
 */
class UNREALBOY_API FUnrealBoyChannel4 : public FUnrealBoyAudioChannel
{
public:
	FUnrealBoyChannel4(FUnrealBoyMotherboard& InMotherboard);
	virtual ~FUnrealBoyChannel4() override = default;

	virtual void Tick() override;
	virtual uint8 GetDigitalOutput() override;
	virtual bool IsEnabled() const override;
	virtual bool IsDACEnabled() const override;
	virtual void Trigger() override;
	virtual void TickLengthCounter() override;
	virtual void TickVolumeEnvelope() override;

	/** Get memory write delegates for motherboard registration */
	TMap<uint16, FUnrealBoyWriteMemoryDelegate> GetMemoryWriteDelegates();

private:
	// LFSR (Linear Feedback Shift Register)
	uint16 LFSR;
	bool bWidthMode; // 7-bit or 15-bit mode

	// Frequency
	uint8 ClockShift;
	uint8 DivisorCode;
	uint16 FrequencyTimer;

	// Length counter
	uint8 LengthCounter;
	bool bLengthEnabled;

	// Volume envelope
	uint8 InitialVolume;
	uint8 CurrentVolume;
	bool bEnvelopeIncrease;
	uint8 EnvelopePeriod;
	uint8 EnvelopeTimer;

	// Memory handlers
	void InitializeMemoryHandlers();
	bool OnWriteNR41(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR42(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR43(uint16 Address, uint8 OldValue, uint8 NewValue);
	bool OnWriteNR44(uint16 Address, uint8 OldValue, uint8 NewValue);

	// Helper functions
	uint16 GetDivisor() const;
	void UpdateLFSR();
}; 
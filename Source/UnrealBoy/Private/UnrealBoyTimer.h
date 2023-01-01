// License: See LICENSE.md file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"

class FUnrealBoyMotherboard;
/**
 * Depending on the TAC register, the timer can run at one of four frequencies
 * 00:   4096 Hz (OSC/1024)
 * 01: 262144 Hz (OSC/16)
 * 10:  65536 Hz (OSC/64)
 * 11:  16384 Hz (OSC/256)
 */
class FUnrealBoyTimer : public IUnrealBoySerializable
{
public:
	FUnrealBoyTimer(FUnrealBoyMotherboard& InMotherboard);

	virtual void Serialize(FArchive& Ar) override;

	/** Read memory by specified address */
	uint8 ReadMemory(uint16 Address);
	
	/** Write memory by specified address */
	void WriteMemory(uint16 Address, uint8 Value);

	uint8 Tick(uint32 Cycles);

	uint32 GetCyclesToInterrupt() const;

protected:
	void Reset();

private:
	FUnrealBoyMotherboard& Motherboard;

	uint8& DIV;
	uint8& TIMA;
	uint8& TMA; 
	uint8& TAC;

	uint32 DIVCounter;
	uint32 TIMACounter;
	TArray<uint16> Dividers;
};

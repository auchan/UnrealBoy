// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"

namespace UnrealBoyAddressNames
{
	constexpr uint16 InterruptVBlank = 0x0040;
	constexpr uint16 InterruptLCDC = 0x0048;
	constexpr uint16 InterruptTimer = 0x0050;
	constexpr uint16 InterruptSerial = 0x0058;
	constexpr uint16 InterruptHighToLow = 0x0060;
	
	constexpr uint16 InterruptFlagRegister = 0xFF0F;
	constexpr uint16 InterruptEnableRegister = 0xFFFF;
}

/**
 * Motherboard
 */
class UNREALBOY_API FUnrealBoyMotherboard
{
	DECLARE_DELEGATE_RetVal_ThreeParams(bool, FWriteMemoryDelegate, uint16 /** Address */, uint8 /** OldValue */, uint8 /** NewValue */);
public:
	FUnrealBoyMotherboard(const TArray<uint8>& InROMData);
	~FUnrealBoyMotherboard();

	/** Read memory by specified address */
	uint8 ReadMemory(uint16 Address);
	
	/** Write memory by specified address */
	void WriteMemory(uint16 Address, uint8 Value);
	
	/**
	 * Write memories by specified memory units
	 * Only used for initialize, will not invoke delegate
	 */
	void WriteMemories(std::initializer_list<FUnrealBoyMemoryUnit> MemoryUnits);

	/** Get memory reference, used to map variable to memory block */
	uint8& GetMemoryRef(uint16 Address);

	bool Tick();

	FString DumpState(const FString& InLabel) const;

private:
	void InitializeMemoryDelegates();
	bool OnDisableBootRom(uint16 Address , uint8 OldValue, uint8 NewValue);
	
private:
	TArray<uint8> MemoryBlock;
	TMap<uint16, FWriteMemoryDelegate> WriteMemoryDelegateMap;

	TSharedPtr<class FUnrealBoyCPU> CPU;
	TSharedPtr<class FUnrealBoyBootRom> BotRoom;
	TSharedPtr<class FUnrealBoyBaseMBC> MBC;

	// states
	bool bBootRomEnabled;
};

﻿// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"

/**
 * The base memory bank controller.
 * MBC chips are located in the game cartridge (that is, not in the Game Boy itself).
 * In each cartridge, the required (or preferred) MBC type should be specified in the byte at $0147 of the ROM.
 */
class FUnrealBoyBaseMBC : public IUnrealBoySerializable
{
public:
	virtual ~FUnrealBoyBaseMBC() override {}

	virtual void Initialize(const TArray<uint8>& InRomData, uint8 InRamCount, bool bIsRTCEnabled);

	virtual void Serialize(FArchive& Ar) override;

	/** Read memory by specified address */
	virtual uint8 ReadMemory(uint16 Address);
	
	/** Write memory by specified address */
	virtual void WriteMemory(uint16 Address, uint8 Value) = 0;

	uint8 ReadFromRomBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress);

	uint8 ReadFromRamBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress);
	
	void WriteToRamBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress, uint8 Value);

	static uint32 GetOffsetToRomBankData(uint8 BankNumber, uint16 Address, uint16 BaseAddress);
	
	static uint32 GetOffsetToRamBankData(uint8 BankNumber, uint16 Address, uint16 BaseAddress);
	
	static void LogWriteToDisabledRAMAddress(uint16 Address);
	
	static void LogWriteToInvalidAddress(uint16 Address);
	
	static void LogReadFromInvalidAddress(uint16 Address);

protected:

	TArray<uint8> RomBankDataArray;
	
	TArray<uint8> RamBankDataArray;

	/** The number of roms */
	uint8 RomBankCount = 0;

	/** The selected rom bank index */
	uint8 RomBankSelection = 1;

	/** The number of rams */
	uint8 RamBankCount = 0;

	/** Whether RAM bank is enabled */ 
	bool bRamBankEnabled = false;
	
	/** The selected ram bank index */
	uint8 RamBankSelection = 0;

	/** Whether realtime clock (RTC) is enabled */ 
	bool bRTCEnabled = false;
};

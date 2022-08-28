// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

/**
 * The base memory bank controller.
 * MBC chips are located in the game cartridge (that is, not in the Game Boy itself).
 * In each cartridge, the required (or preferred) MBC type should be specified in the byte at $0147 of the ROM.
 */
class FUnrealBoyBaseMBC
{
public:
	virtual ~FUnrealBoyBaseMBC() {}

	virtual void Initialize(const TArray<uint8>& InRomData, uint8 InRamCount);

	/** Read memory by specified address */
	virtual uint8 ReadMemory(uint16 Address);
	
	/** Write memory by specified address */
	virtual void WriteMemory(uint16 Address, uint8 Value);

	uint8 ReadFromRomBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress);

	uint8 ReadFromRamBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress);
	
	void WriteToRamBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress, uint8 Value);

	static uint32 GetOffsetToRomBankData(uint8 BankNumber, uint16 Address, uint16 BaseAddress);
	
	static uint32 GetOffsetToRamBankData(uint8 BankNumber, uint16 Address, uint16 BaseAddress);

protected:

	TArray<uint8> RomBankDataArray;
	
	TArray<uint8> RamBankDataArray;

	uint8 RomBankCount = 0;
	
	uint8 RamBankCount = 0;
};

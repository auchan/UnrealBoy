// License: See LICENSE.txt file


#include "UnrealBoyBaseMBC.h"

#include "UnrealBoyCartridge.h"

void FUnrealBoyBaseMBC::Initialize(const TArray<uint8>& InRomData, uint8 InRamCount)
{
	RomBankDataArray = InRomData;
	RomBankCount = RomBankDataArray.Num() / FUnrealBoyCartridge::ROM_BANK_SIZE;
	checkf(RomBankDataArray.Num() % FUnrealBoyCartridge::ROM_BANK_SIZE == 0, TEXT("Unexcepted ROM data, size = %d"), RomBankDataArray.Num());
	
	RamBankDataArray.SetNumZeroed(InRamCount * FUnrealBoyCartridge::RAM_BANK_SIZE);
	RamBankCount = InRamCount;
}

uint8 FUnrealBoyBaseMBC::ReadMemory(uint16 Address)
{
	// Add common implementation
	return 0;
}

void FUnrealBoyBaseMBC::WriteMemory(uint16 Address, uint8 Value)
{
	
}

uint8 FUnrealBoyBaseMBC::ReadFromRomBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress)
{
	const uint32 Offset = GetOffsetToRomBankData(BankNumber, Address, BaseAddress);
	return RomBankDataArray[Offset];
}

uint8 FUnrealBoyBaseMBC::ReadFromRamBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress)
{
	const uint32 Offset = GetOffsetToRamBankData(BankNumber, Address, BaseAddress);
	return RamBankDataArray[Offset];
}

void FUnrealBoyBaseMBC::WriteToRamBank(uint8 BankNumber, uint16 Address, uint16 BaseAddress, uint8 Value)
{
	const uint32 Offset = GetOffsetToRamBankData(BankNumber, Address, BaseAddress);
	RamBankDataArray[Offset] = Value;
}

uint32 FUnrealBoyBaseMBC::GetOffsetToRomBankData(uint8 BankNumber, uint16 Address, uint16 BaseAddress)
{
	const uint32 Offset = BankNumber * FUnrealBoyCartridge::ROM_BANK_SIZE + Address - BaseAddress;
	return Offset;
}

uint32 FUnrealBoyBaseMBC::GetOffsetToRamBankData(uint8 BankNumber, uint16 Address, uint16 BaseAddress)
{
	const uint32 Offset = BankNumber * FUnrealBoyCartridge::RAM_BANK_SIZE + Address - BaseAddress;
	return Offset;
}

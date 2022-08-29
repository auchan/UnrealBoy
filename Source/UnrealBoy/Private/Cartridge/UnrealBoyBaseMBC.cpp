// License: See LICENSE.txt file


#include "UnrealBoyBaseMBC.h"

#include "UnrealBoyCartridge.h"
#include "UnrealBoyLog.h"

void FUnrealBoyBaseMBC::Initialize(const TArray<uint8>& InRomData, uint8 InRamCount, bool bIsRTCEnabled)
{
	RomBankDataArray = InRomData;
	RomBankCount = RomBankDataArray.Num() / FUnrealBoyCartridge::ROM_BANK_SIZE;
	checkf(RomBankDataArray.Num() % FUnrealBoyCartridge::ROM_BANK_SIZE == 0, TEXT("Unexcepted ROM data, size = %d"), RomBankDataArray.Num());
	
	RamBankDataArray.SetNumZeroed(InRamCount * FUnrealBoyCartridge::RAM_BANK_SIZE);
	RamBankCount = InRamCount;

	bRTCEnabled = bIsRTCEnabled;
}

uint8 FUnrealBoyBaseMBC::ReadMemory(uint16 Address)
{
	if (Address < 0x4000)
	{
		return ReadFromRomBank(0, Address, 0x0);
	}
	else if (0x4000 <= Address && Address < 0x8000)
	{
		return ReadFromRomBank(RomBankSelection % RomBankCount, Address, 0x4000);
	}
	else if (0xA000 <= Address && Address < 0xC000)
	{
		if (!bRamBankEnabled)
		{
			return 0xFF;
		}

		if (bRTCEnabled && 0x08 <= RamBankSelection && RamBankSelection <= 0x0C)
		{
			checkf(false, TEXT("TODO: need implement RTC"))
			return 0xFF;
		}
		
		return ReadFromRamBank(RamBankSelection % RamBankCount, Address, 0xA000);
	}
	else
	{
		LogReadFromInvalidAddress(Address);
		return 0xFF;
	}
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

void FUnrealBoyBaseMBC::LogWriteToDisabledRAMAddress(uint16 Address)
{
	UE_LOG(LogUnrealBoy, Error, TEXT("Attempting write to RAM (address: %04X), but the ram bank has not been enabled"), Address);
}

void FUnrealBoyBaseMBC::LogWriteToInvalidAddress(uint16 Address)
{
	UE_LOG(LogUnrealBoy, Error, TEXT("Attempting write to invalid address: %04X"), Address);
}

void FUnrealBoyBaseMBC::LogReadFromInvalidAddress(uint16 Address)
{
	UE_LOG(LogUnrealBoy, Error, TEXT("Attempting read from invalid address: %04X"), Address);
}

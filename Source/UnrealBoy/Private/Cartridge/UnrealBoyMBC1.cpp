// License: See LICENSE.txt file


#include "UnrealBoyMBC1.h"
#include "UnrealBoyLog.h"

void FUnrealBoyMBC1::Serialize(FArchive& Ar)
{
	FUnrealBoyBaseMBC::Serialize(Ar);

	Ar << BankSelectRegister1;
	Ar << BankSelectRegister2;
	Ar << BankingMode;
}

uint8 FUnrealBoyMBC1::ReadMemory(uint16 Address)
{
	if (Address < 0x4000) // Read from rom bank #0 or selectable rom banks
	{
		uint16 RomBankNumber = 0;
		if (BankingMode == 1)
		{
			RomBankNumber = (BankSelectRegister2 << 5) % RomBankCount;
		}
		return ReadFromRomBank(RomBankNumber, Address, 0x0);
	}
	else if (Address < 0x8000) // Read from selectable rom banks
	{
		const uint16 RomBankNumber = (((BankSelectRegister2 << 5) % RomBankCount) | BankSelectRegister1) % RomBankCount;
		return ReadFromRomBank(RomBankNumber, Address, 0x4000);
	}
	else if (0xA000 <= Address && Address < 0xC000) // Read from ram banks
	{
		if (!bRamBankEnabled)
		{
			return 0xFF;
		}
		const uint8 RamBankNumber = (BankingMode == 1 ? BankSelectRegister2 : 0) % RamBankCount;
		return ReadFromRamBank(RamBankNumber, Address, 0xA000);
	}
	else
	{
		LogReadFromInvalidAddress(Address);
		return 0xFF;
	}
}

void FUnrealBoyMBC1::WriteMemory(uint16 Address, uint8 Value)
{
	if (Address < 0x2000) // Enable or disable RAM
	{
		// Enabled by writing 0xA to this address space.
		// Any value with 0xA in the lower 4 bits enables the RAM attached to the MBC and any other value disables the RAM.
		bRamBankEnabled = (Value & 0xF) == 0xA;
	}
	else if (Address < 0x4000) // Set ROM bank number
	{
		// Only lower 5 bits used
		Value &= 0b11111;
		// If this register is set to 0x0, it behaves as if it is set to 0x1.
		// This means you cannot duplicate bank #0 into both the 0000-3FFF and 4000-7FFF ranges by setting this register to 0x0.
		if (Value == 0)
		{
			Value = 1;
		}
		BankSelectRegister1 = Value;
	}
	else if (Address < 0x6000) // Set RAM bank number or upper bits of ROM bank number
	{
		BankSelectRegister2 = Value & 0b11;
	}
	else if (Address < 0x8000) // Select banking mode
	{
		BankingMode = Value & 0b1;
	}
	else if (0xA000 <= Address && Address < 0xC000) // Write to RAM banks
	{
		if(bRamBankEnabled)
		{
			const uint8 RamBankNumber = (BankingMode == 1 ? BankSelectRegister2 : 0) % RamBankCount;
			WriteToRamBank(RamBankNumber, Address, 0xA000, Value);	
		}
		else
		{
			LogWriteToDisabledRAMAddress(Address);
		}
	}
	else
	{
		LogWriteToInvalidAddress(Address);
	}
}

// License: See LICENSE.md file


#include "UnrealBoyMBC2.h"

uint8 FUnrealBoyMBC2::ReadMemory(uint16 Address)
{
	if (Address < 0x4000) // Read from rom bank #0
	{
		return ReadFromRomBank(0, Address, 0x0);
	}
	else if (0x4000 <= Address && Address < 0x8000) // Read from selectable rom banks
	{
		return ReadFromRomBank(RomBankSelection % RomBankCount, Address, 0x4000);
	}
	else if (0xA000 <= Address && Address < 0xC000) // Read from ram banks
	{
		if (!bRamBankEnabled)
		{
			return 0xFF;
		}
		// Only the lower 4 bits of the bit octets in this memory area are used
		return ReadFromRamBank(0, Address % 512, 0x0) | 0xF0;
	}
	else
	{
		LogReadFromInvalidAddress(Address);
		return 0xFF;
	}
}

void FUnrealBoyMBC2::WriteMemory(uint16 Address, uint8 Value)
{
	if (Address < 0x4000)
	{
		Value &= 0x0F;
		if ((Address & 0x100) == 0)
		{
			// When the least significant bit of the upper address byte is zero, the value that is written controls whether the RAM is enabled
			bRamBankEnabled = (Value == 0x0A);
		}
		else
		{
			// Otherwise the value that is written controls the selected ROM bank at 4000-7FFF
			// If bank 0 is written, the resulting bank will be bank 1 instead.
			if (Value == 0)
			{
				Value = 1;
			}
			RomBankSelection = Value;
		}
	}
	else if (0xA000 <= Address && Address < 0xC000)
	{
		if (bRamBankEnabled)
		{
			WriteToRamBank(0, Address % 512, 0x0, Value | 0xF0);
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

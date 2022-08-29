// License: See LICENSE.md file


#include "UnrealBoyMBC3.h"
#include "UnrealBoyLog.h"

void FUnrealBoyMBC3::WriteMemory(uint16 Address, uint8 Value)
{
	if (Address < 0x2000) // // Enable or disable RAM
	{
		bRamBankEnabled = (Value & 0x0F) == 0x0A;
	}
	else if (0x2000 <= Address && Address < 0x4000) // Set ROM bank number
	{
		// The whole 7 bits of the ROM Bank Number are written directly to this address.
		Value &= 0x7F;
		if (Value == 0)
		{
			Value = 1;
		}
		RomBankSelection = Value;
	}
	else if (0x4000 <= Address && Address < 0x6000) // Set RAM bank number or RTC register
	{
		RamBankSelection = Value;
	}
	else if (0x6000 <= Address && Address < 0x8000) // Issue RTC command
	{
		if (bRTCEnabled)
		{
			checkf(false, TEXT("TODO: RTC support"));
		}
		else
		{
			UE_LOG(LogUnrealBoy, Warning, TEXT("RTC not present. Game tried to issue RTC command: 0x%04X, 0x%02X"), Address, Value)
		}
	}
	else if (0xA000 <= Address && Address < 0xC000)
	{
		if (bRamBankEnabled)
		{
			if (RamBankSelection <= 0x03)
			{
				WriteToRamBank(RamBankSelection % RamBankCount, Address, 0xA000, Value);
			}
			else if (0x08 <= RamBankSelection && RamBankSelection <= 0x0C)
			{
				checkf(false, TEXT("TODO: RTC support"));
			}
			else
			{
				UE_LOG(LogUnrealBoy, Error, TEXT("Invalid RAM bank selected: %d"), RamBankSelection)
			}
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

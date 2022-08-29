// License: See LICENSE.txt file


#include "UnrealBoyMBCRomOnly.h"

void FUnrealBoyMBCRomOnly::WriteMemory(uint16 Address, uint8 Value)
{
	if (0x2000 <= Address && Address < 0x4000)
	{
		// Ignore switch rom bank
	}
	else if (0xA000 <= Address && Address < 0xC000)
	{
		return WriteToRamBank(0, Address, 0xA000, Value);
	}
	else
	{
		LogWriteToInvalidAddress(Address);
	}
}

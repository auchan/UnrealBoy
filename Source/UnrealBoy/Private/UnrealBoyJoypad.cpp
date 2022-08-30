// License: See LICENSE.md file


#include "UnrealBoyJoypad.h"
#include "UnrealBoyMotherboard.h"

FUnrealBoyJoyPad::FUnrealBoyJoyPad(FUnrealBoyMotherboard& InMotherboard)
	: Motherboard(InMotherboard)
	, JoyPadRegister(UNREALBOY_GET_MEMORY_REF(JoyPad))
{
	JoyPadRegister = 0xFF;
	PressedDirectionalKeys = 0xFF;
	PressedFunctionalKeys = 0xFF;
}

void FUnrealBoyJoyPad::WriteMemory(uint16 Address, uint8 Value)
{
	// Select Direction buttons (0=Select)
	const bool P14 = (Value >> 4) & 1;
	// Select Action buttons (0=Select)
	const bool P15 = (Value >> 5) & 1;

	// Make first 4 and last 2 bits true, while keeping selected bits
	Value = 0xFF & (Value | 0b11001111);
	if (P14 && P15)
	{
		// Not select any buttons
		return;	
	}
	else if (!P14 && !P15)
	{
		// Select both buttons, not allowed
		return;	
	}
	else if (!P14)
	{
		// Select direction button
		JoyPadRegister = Value & PressedDirectionalKeys;
	}
	else if (!P15)
	{
		// Select action button
		JoyPadRegister = Value & PressedFunctionalKeys;
	}
}

// License: See LICENSE.md file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"

class FUnrealBoyMotherboard;

/**
 * The input register
 */
class FUnrealBoyJoyPad
{
public:
	FUnrealBoyJoyPad(FUnrealBoyMotherboard& InMotherboard);

	/** Write memory by specified address */
	void WriteMemory(uint16 Address, uint8 Value);
	
	/** On key event */
	void OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent);

protected:
	void SetKeyBit(uint8& Keys, uint8 Bit, bool bReset);
	
private:
	/** Reference to motherboard */
	FUnrealBoyMotherboard& Motherboard;

	/**
	 * Bit 7 - Not used (No$GMB)
	 * Bit 6 - Not used (No$GMB)
	 * Bit 5 - P15 out port Action buttons		(0=Select)
	 * Bit 4 - P14 out port Direction buttons	(0=Select)
	 * Bit 3 - P13 in port Down  or Start		(0=Pressed)
	 * Bit 2 - P12 in port Up    or Select		(0=Pressed)
	 * Bit 1 - P11 in port Left  or B			(0=Pressed)
	 * Bit 0 - P10 in port Right or A			(0=Pressed)
	 */
	uint8& JoyPadRegister;

	/** Pressed keys*/
	uint8 PressedDirectionalKeys;
	uint8 PressedFunctionalKeys;
};

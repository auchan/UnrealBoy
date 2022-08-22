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

	/** Read memory by specified address */
	virtual uint8 ReadMemory(uint16 Address);
	
	/** Write memory by specified address */
	virtual void WriteMemory(uint16 Address, uint8 Value);
};

// License: See LICENSE.md file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyBaseMBC.h"

/**
 * MBC type 3:
 * Max 2MByte ROM and/or 32KByte RAM and Timer
 */
class FUnrealBoyMBC3 : public FUnrealBoyBaseMBC
{
public:
	
	virtual void WriteMemory(uint16 Address, uint8 Value) override;
};

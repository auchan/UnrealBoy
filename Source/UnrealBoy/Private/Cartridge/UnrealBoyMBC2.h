// License: See LICENSE.md file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyBaseMBC.h"

/**
 * MBC type 2:
 * Max 256 KiB ROM and 512x4 bits RAM
 */
class FUnrealBoyMBC2 : public FUnrealBoyBaseMBC
{
public:
	
	virtual uint8 ReadMemory(uint16 Address) override;

	virtual void WriteMemory(uint16 Address, uint8 Value) override;
};

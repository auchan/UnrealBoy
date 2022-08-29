// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyBaseMBC.h"

/**
 * No MBC, 32KB ROM only
 */
class FUnrealBoyMBCRomOnly : public FUnrealBoyBaseMBC
{
public:
	
	virtual void WriteMemory(uint16 Address, uint8 Value) override;
};

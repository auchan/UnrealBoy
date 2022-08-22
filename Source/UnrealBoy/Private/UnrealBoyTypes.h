// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

class FUnrealBoyMemoryUnit
{
public:
	FUnrealBoyMemoryUnit() : Address(0), Value(0) {}
	FUnrealBoyMemoryUnit(const uint16 InAddress, const uint8 InValue) : Address(InAddress), Value(InValue) {}
	
	uint16 Address;
	uint8 Value;
};
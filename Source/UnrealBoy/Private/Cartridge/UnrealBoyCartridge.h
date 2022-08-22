// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

class FUnrealBoyBaseMBC;
/**
 * Provide additional ROM and RAM.
 * All the games must load from a cartridge
 */
class FUnrealBoyCartridge
{
public:
	static TSharedPtr<FUnrealBoyBaseMBC> LoadROM(const TArray<uint8>& InROMData);

	static TSharedPtr<FUnrealBoyBaseMBC> CreateMBC(const int32 CartridgeType);
};

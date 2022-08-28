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

	static TSharedPtr<FUnrealBoyBaseMBC> CreateMBC(const int32 CartridgeType, const TArray<uint8>& InROMData);

	inline static uint16 ROM_BANK_SIZE = 16 * 1024; // 16KB 
	inline static uint16 RAM_BANK_SIZE = 8 * 1024; // 8KB 
};

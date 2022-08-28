// License: See LICENSE.txt file


#include "UnrealBoyCartridge.h"

#include "UnrealBoyMBC1.h"
#include "UnrealBoyMBCRomOnly.h"

namespace UnrealBoyCartridge
{
	TMap<uint8, uint8> RamBankNumberTable = {
		{0x0, 1}, // Set to 1 to avoid calculation error
		{0x1, 1},
		{0x2, 1},
		{0x3, 4},
		{0x4, 16},
		{0x5, 8},
	};
}

TSharedPtr<FUnrealBoyBaseMBC> FUnrealBoyCartridge::LoadROM(const TArray<uint8>& InROMData)
{
	// Parse ROM data, construct MBC according to cartridge type and predefined parameters.
	constexpr uint16 MBCTypeAddress = 0x0147;
	check(InROMData.IsValidIndex(MBCTypeAddress));
	const int32 CartridgeType = InROMData[MBCTypeAddress];
	
	return CreateMBC(CartridgeType, InROMData);
}

TSharedPtr<FUnrealBoyBaseMBC> FUnrealBoyCartridge::CreateMBC(const int32 CartridgeType, const TArray<uint8>& InROMData)
{
	// TODO: use a predefined table
	TSharedPtr<FUnrealBoyBaseMBC> MBC;
	if (CartridgeType == 0)
	{
		MBC = MakeShareable(new FUnrealBoyMBCRomOnly());
	}
	else if (CartridgeType == 1)
	{
		MBC = MakeShareable(new FUnrealBoyMBC1());
	}

	if (MBC)
	{
		MBC->Initialize(InROMData, UnrealBoyCartridge::RamBankNumberTable[CartridgeType]);
	}

	return MBC;
}

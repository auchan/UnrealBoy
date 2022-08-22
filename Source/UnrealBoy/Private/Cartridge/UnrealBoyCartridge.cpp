// License: See LICENSE.txt file


#include "UnrealBoyCartridge.h"

#include "UnrealBoyMBC1.h"
#include "UnrealBoyMBCRomOnly.h"

TSharedPtr<FUnrealBoyBaseMBC> FUnrealBoyCartridge::LoadROM(const TArray<uint8>& InROMData)
{
	// Parse ROM data, construct MBC according to cartridge type and predefined parameters.
	constexpr uint16 MBCTypeAddress = 0x0147;
	check(InROMData.IsValidIndex(MBCTypeAddress));
	const int32 CartridgeType = InROMData[MBCTypeAddress];
	
	return CreateMBC(CartridgeType);
}

TSharedPtr<FUnrealBoyBaseMBC> FUnrealBoyCartridge::CreateMBC(const int32 CartridgeType)
{
	// TODO: use a predefined table
	if (CartridgeType == 0)
	{
		return MakeShareable(new FUnrealBoyMBCRomOnly());
	}
	else if (CartridgeType == 1)
	{
		return MakeShareable(new FUnrealBoyMBC1());
	}

	return nullptr;
}

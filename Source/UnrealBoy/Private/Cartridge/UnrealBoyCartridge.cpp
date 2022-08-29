// License: See LICENSE.txt file


#include "UnrealBoyCartridge.h"

#include "UnrealBoyMBC1.h"
#include "UnrealBoyMBC2.h"
#include "UnrealBoyMBC3.h"
#include "UnrealBoyMBCRomOnly.h"

namespace UnrealBoyCartridge
{
	struct FCartridgeInfo
	{
		FCartridgeInfo() : MBCConstructFunc(nullptr), bRTCEnabled(false) {}
		FCartridgeInfo(const TFunction<TSharedPtr<FUnrealBoyBaseMBC>()>& InMBCFunc, bool bIsRTCEnabled = false)
			: MBCConstructFunc(InMBCFunc)
			, bRTCEnabled(bIsRTCEnabled)
		{
		}
		
		TFunction<TSharedPtr<FUnrealBoyBaseMBC>()> MBCConstructFunc;

		bool bRTCEnabled;
	};
	
	template <typename T>
	TSharedPtr<FUnrealBoyBaseMBC> CreateMBC()
	{
		return MakeShareable(new T());
	}
	
	TMap<uint8, FCartridgeInfo> CartridgeInfoTable = {
		{0x00, {CreateMBC<FUnrealBoyMBCRomOnly>}},
		{0x01, {CreateMBC<FUnrealBoyMBC1>}},
		{0x02, {CreateMBC<FUnrealBoyMBC1>}},
		{0x03, {CreateMBC<FUnrealBoyMBC1>}},
		{0x05, {CreateMBC<FUnrealBoyMBC2>}},
		{0x06, {CreateMBC<FUnrealBoyMBC2>}},
		{0x08, {CreateMBC<FUnrealBoyMBCRomOnly>}},
		{0x09, {CreateMBC<FUnrealBoyMBCRomOnly>}},
		{0x0F, {CreateMBC<FUnrealBoyMBC3>, true}},
		{0x10, {CreateMBC<FUnrealBoyMBC3>, true}},
		{0x11, {CreateMBC<FUnrealBoyMBC3>}},
		{0x12, {CreateMBC<FUnrealBoyMBC3>}},
		{0x13, {CreateMBC<FUnrealBoyMBC3>}},
	};
	
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
	const uint8 CartridgeType = InROMData[MBCTypeAddress];
	
	return CreateMBC(CartridgeType, InROMData);
}

TSharedPtr<FUnrealBoyBaseMBC> FUnrealBoyCartridge::CreateMBC(const int32 CartridgeType, const TArray<uint8>& InROMData)
{
	const UnrealBoyCartridge::FCartridgeInfo CartridgeInfo = UnrealBoyCartridge::CartridgeInfoTable.FindRef(CartridgeType);
	TSharedPtr<FUnrealBoyBaseMBC> MBC;
	if (CartridgeInfo.MBCConstructFunc)
	{
		MBC = CartridgeInfo.MBCConstructFunc();
	}
	checkf(MBC, TEXT("Unsupported cartridge type: %d"), CartridgeType);

	constexpr uint16 RAMSizeAddress = 0x0149;
	const uint8 RAMSizeType = InROMData[RAMSizeAddress];
	MBC->Initialize(InROMData, UnrealBoyCartridge::RamBankNumberTable[RAMSizeType], CartridgeInfo.bRTCEnabled);
	return MBC;
}

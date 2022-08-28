// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"

#define UNREALBOY_GET_MEMORY_REF(AddressName) Motherboard.GetMemoryRef(UnrealBoyAddressNames::AddressName)
#define UNREALBOY_GET_MEMORY_RANGE_VIEW(BeginAddressName, EndAddressName) Motherboard.GetMemoryRangeView(UnrealBoyAddressNames::BeginAddressName, UnrealBoyAddressNames::EndAddressName)

/**
 * Motherboard
 */
class UNREALBOY_API FUnrealBoyMotherboard
{
	DECLARE_DELEGATE_RetVal_ThreeParams(bool, FWriteMemoryDelegate, uint16 /** Address */, uint8 /** OldValue */, uint8 /** NewValue */);
public:
	FUnrealBoyMotherboard(const TArray<uint8>& InROMData);
	~FUnrealBoyMotherboard();

	/** Read memory by specified address */
	uint8 ReadMemory(uint16 Address);
	
	/** Write memory by specified address */
	void WriteMemory(uint16 Address, uint8 Value);
	
	/**
	 * Write memories by specified memory units
	 * Only used for initialize, will not invoke delegate
	 */
	void WriteMemories(std::initializer_list<FUnrealBoyMemoryUnit> MemoryUnits);

	/** Get memory reference, used to map variable to memory block */
	uint8& GetMemoryRef(uint16 Address);

	/** Get memory range view */
	TArrayView<uint8> GetMemoryRangeView(uint16 BeginAddress, uint16 EndAddress);

	/** Get screen buffer */
	const TArray<FColor>& GetScreenBuffer() const;
	
	/** Get Tile Cache */
	const TArray<FColor>& GetTileMap1Buffer() const;

	bool Tick();

	void TransferDMA(uint8 SrcAddress);

	FString DumpState(const FString& InLabel) const;

private:
	void InitializeMemoryDelegates();
	bool OnDisableBootRom(uint16 Address , uint8 OldValue, uint8 NewValue);
	
private:
	TArray<uint8> MemoryBlock;
	TMap<uint16, FWriteMemoryDelegate> WriteMemoryDelegateMap;

	TSharedPtr<class FUnrealBoyCPU> CPU;
	TSharedPtr<class FUnrealBoyBootRom> BotRoom;
	TSharedPtr<class FUnrealBoyBaseMBC> MBC;
	TSharedPtr<class FUnrealBoyLCD> LCD;
	TSharedPtr<class FUnrealBoyTimer> Timer;

	// states
	bool bBootRomEnabled;
};

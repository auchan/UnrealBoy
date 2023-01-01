// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"

class FUnrealBoyMotherboard;

namespace EUnrealBoyEmulatorFlags
{
	enum Type
	{
		None = 0,
		AutoLoadWhenStart = 1 << 0,
		AutoSaveWhenStop = 1 << 1,
	};
}

/**
 * 
 */
class UNREALBOY_API FUnrealBoyEmulator : public IUnrealBoySerializable
{
public:
	FUnrealBoyEmulator(uint32 InFlags = 0)
		: Flags(InFlags)
	{}

	int32 Start(const FString& InROMFilePath);

	void Tick(float InDeltaTime);

	int32 Stop();

	/** Whether this emulator is setup properly */
	bool IsValid() const;
	
	/** Get screen buffer */
	const TArray<FColor>& GetScreenBuffer() const;
	
	/** Get tile cache */
	const TArray<FColor>& GetTileMap1Buffer() const;

	/** Get on frame done delegate */
	FSimpleMulticastDelegate& GetOnFrameDoneDelegate() const;

	/** On key event */
	void OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent);

	/** Load data from file */
	void LoadData(const FString& InFilePath);

	/** Save data to file */
	void SaveData(const FString& InFilePath);

	/** Serialize to archive or deserialize from archive (depend on archive type)  */
	virtual void Serialize(FArchive& Ar) override;

protected:
	bool LoadROMFile(const FString& InROMFilePath, TArray<uint8>& OutLoadedData) const;

	bool HasAnyFlags(uint32 InFlags) const;
	
protected:
	/** The emulated motherboard */
	TSharedPtr<FUnrealBoyMotherboard> Motherboard;

	/** Various emulator settings flags*/
	uint32 Flags = 0;

	/** The save data file path calculated by ROM file path */
	FString SaveDataFilePath;
};

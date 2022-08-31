﻿// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyTypes.h"

class FUnrealBoyMotherboard;

/**
 * 
 */
class UNREALBOY_API FUnrealBoyEmulator
{
public:
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
protected:
	bool LoadROMFile(const FString& InROMFilePath, TArray<uint8>& OutLoadedData) const;
	
protected:
	/** The emulated motherboard */
	TSharedPtr<FUnrealBoyMotherboard> Motherboard;
};

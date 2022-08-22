﻿// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

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

protected:
	bool LoadROMFile(const FString& InROMFilePath, TArray<uint8>& OutLoadedData) const;
	
protected:
	/** The emulated motherboard */
	TSharedPtr<FUnrealBoyMotherboard> Motherboard;
};

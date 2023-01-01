// License: See LICENSE.md file

#pragma once

#include "CoreMinimal.h"

class FUnrealBoyEmulator;
/**
 * 
 */
class FUnrealBoyUtility
{
public:
	static UTexture2D* CreateTextureForScreenBuffer(FUnrealBoyEmulator& Emulator);

	static void UpdateTextureForScreenBuffer(FUnrealBoyEmulator& Emulator, UTexture2D* Texture);

	static UTexture2D* CreateBufferTexture(const TArray<FColor>& Buffer, uint32 ImageWidth, uint32 ImageHeight, FName BaseName = NAME_None);

	static void UpdateBufferTexture(const TArray<FColor>& Buffer, uint32 ImageWidth, uint32 ImageHeight, UTexture2D* Texture);
};

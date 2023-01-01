// License: See LICENSE.md file


#include "UnrealBoyUtility.h"

#include "ImageUtils.h"
#include "UnrealBoyEmulator.h"
#include "UnrealBoyLCD.h"

UTexture2D* FUnrealBoyUtility::CreateTextureForScreenBuffer(FUnrealBoyEmulator& Emulator)
{
	const TArray<FColor> ScreenBuffer = Emulator.GetScreenBuffer();
	return CreateBufferTexture(ScreenBuffer, FUnrealBoyLCD::NUM_COLS, FUnrealBoyLCD::NUM_ROWS, TEXT("ScreenBuffer"));
}

void FUnrealBoyUtility::UpdateTextureForScreenBuffer(FUnrealBoyEmulator& Emulator, UTexture2D* Texture)
{
	const TArray<FColor> ScreenBuffer = Emulator.GetScreenBuffer();
	UpdateBufferTexture(ScreenBuffer, FUnrealBoyLCD::NUM_COLS, FUnrealBoyLCD::NUM_ROWS, Texture);
}

UTexture2D* FUnrealBoyUtility::CreateBufferTexture(const TArray<FColor>& Buffer, uint32 ImageWidth, uint32 ImageHeight, FName BaseName)
{
	UPackage* Package = GetTransientPackage();
	const FName TextureName = MakeUniqueObjectName(Package, UTexture2D::StaticClass(), BaseName);
	FCreateTexture2DParameters Parameters;
	Parameters.CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
	return FImageUtils::CreateTexture2D(ImageWidth, ImageHeight, Buffer, Package, TextureName.ToString(), RF_Transient, Parameters);
}

void FUnrealBoyUtility::UpdateBufferTexture(const TArray<FColor>& Buffer, uint32 ImageWidth, uint32 ImageHeight, UTexture2D* Texture)
{
	if (!Texture)
	{
		return;
	}

	// TODO: Add data validation

	if (FTexturePlatformData* TexPlatformData = Texture->GetPlatformData())
	{
		if (TexPlatformData->Mips.Num() > 0)
		{
			uint8* MipData = static_cast<uint8*>(TexPlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
			for (uint32 Y = 0; Y < ImageHeight; Y++)
			{
				uint8* DestPtr = &MipData[(ImageHeight - 1 - Y) * ImageWidth * sizeof(FColor)];
				FColor* SrcPtr = const_cast<FColor*>(&Buffer[(ImageHeight - 1 - Y) * ImageWidth]);
				for (uint32 X = 0; X < ImageWidth; X++)
				{
					*DestPtr++ = SrcPtr->B;
					*DestPtr++ = SrcPtr->G;
					*DestPtr++ = SrcPtr->R;
					*DestPtr++ = 0xFF;
					SrcPtr++;
				}
			}
			Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
			Texture->UpdateResource();
		}
	}
}
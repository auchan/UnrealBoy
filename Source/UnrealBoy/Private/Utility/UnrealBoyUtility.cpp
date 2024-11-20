// License: See LICENSE.md file


#include "UnrealBoyUtility.h"

#include "AssetCompilingManager.h"
#include "ImageUtils.h"
#include "UnrealBoyEmulator.h"
#include "UnrealBoyLCD.h"
#include "UnrealBoyLog.h"
#include "Engine/TextureRenderTarget2D.h"

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
	Parameters.CompressionSettings = TC_VectorDisplacementmap;
	Parameters.MipGenSettings = TMGS_NoMipmaps;
	Parameters.bDeferCompression = true;
	Parameters.bSRGB = false;
	UTexture2D* Texture2D = FImageUtils::CreateTexture2D(ImageWidth, ImageHeight, Buffer, Package, TextureName.ToString(), RF_Transient, Parameters);
	Texture2D->UpdateResource();
	return Texture2D;
}

void FUnrealBoyUtility::UpdateBufferTexture(const TArray<FColor>& Buffer, uint32 ImageWidth, uint32 ImageHeight, UTexture2D* Texture2D)
{
	if (!Texture2D)
	{
		return;
	}

	// TODO: Add data validation
	if (FTexturePlatformData* TexPlatformData = Texture2D->GetPlatformData())
	{
		if (TexPlatformData->Mips.Num() > 0)
		{
			uint8* MipData = static_cast<uint8*>(TexPlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
			if (MipData)
			{
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
			}
			else
			{
				UE_LOG(LogUnrealBoy, Warning, TEXT("Failed to lock texture data"));
			}

			Texture2D->GetPlatformData()->Mips[0].BulkData.Unlock();
			Texture2D->UpdateResource();
		}
	}
}
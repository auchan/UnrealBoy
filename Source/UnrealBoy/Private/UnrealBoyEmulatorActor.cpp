// License: See LICENSE.txt file


#include "UnrealBoyEmulatorActor.h"

#include "ImageUtils.h"
#include "UnrealBoyLCD.h"
#include "UnrealBoyMotherboard.h"


// Sets default values
AUnrealBoyEmulatorActor::AUnrealBoyEmulatorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUnrealBoyEmulatorActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!CartridgeFilePath.FilePath.IsEmpty())
	{
		Emulator.Start(CartridgeFilePath.FilePath);
	}
}

void AUnrealBoyEmulatorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Emulator.Stop();
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AUnrealBoyEmulatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static float FrameTime = 1.f / 60;
	
	AccumulatedTime += DeltaTime;
	if (!Emulator.IsValid())
	{
		return;
	}

	if (AccumulatedTime >= FrameTime)
	{
		AccumulatedTime = FMath::Fmod(AccumulatedTime, FrameTime);
		Emulator.Tick(DeltaTime);
		CaptureScreen();
	}
}

void AUnrealBoyEmulatorActor::CaptureScreen()
{
	TArray<FColor> ScreenBuffer = Emulator.GetScreenBuffer();
	CopyImageBufferToTexture(FUnrealBoyLCD::NUM_COLS, FUnrealBoyLCD::NUM_ROWS, ScreenBuffer, ScreenTexture);
	
	// TArray<FColor> TileMap1Buffer = Emulator.GetTileMap1Buffer();
	// CopyImageBufferToTexture(FUnrealBoyLCD::TILEMAP_WIDTH, FUnrealBoyLCD::TILEMAP_HEIGHT, TileMap1Buffer, TileMap1Texture);
	
	// TArray<FColor> TileMap2Buffer = Emulator.GetTileMap2Buffer();
	// CopyImageBufferToTexture(FUnrealBoyLCD::TILEMAP_WIDTH, FUnrealBoyLCD::TILEMAP_HEIGHT, TileMap2Buffer, TileMap1Texture);
}

void AUnrealBoyEmulatorActor::CopyImageBufferToTexture(uint32 ImageWidth, uint32 ImageHeight,
	const TArray<FColor>& Buffer, UTexture2D*& InScreenTexture)
{
	if (!InScreenTexture)
	{
		UPackage* Package = GetTransientPackage();
		FName TextureName = MakeUniqueObjectName(Package, UTexture2D::StaticClass(), TEXT("T_ScreenCapture"));
		FCreateTexture2DParameters Parameters;
		Parameters.CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
		InScreenTexture = FImageUtils::CreateTexture2D(ImageWidth, ImageHeight, Buffer, Package, TextureName.ToString(), RF_Public | RF_Standalone | RF_Transient, Parameters);

		if (ScreenDisplayActor)
		{
			UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(ScreenDisplayActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if (StaticMeshComp)
			{
				UMaterialInterface* Mat = StaticMeshComp->GetMaterial(0);
				if (Mat)
				{
					UMaterialInstanceDynamic* MatInstanceDynamic = Cast<UMaterialInstanceDynamic>(Mat);
					if (!MatInstanceDynamic)
					{
						MatInstanceDynamic = UMaterialInstanceDynamic::Create(Mat, Mat->GetOuter());
					}

					if (MatInstanceDynamic)
					{
						MatInstanceDynamic->SetTextureParameterValue(TEXT("ScreenTexture"), InScreenTexture);
						StaticMeshComp->SetMaterial(0, MatInstanceDynamic);
					}
				}
			}
		}
	}
	else
	{
		if (FTexturePlatformData* TexPlatformData = InScreenTexture->GetPlatformData())
		{
			if (TexPlatformData->Mips.Num() > 0)
			{
				uint8* MipData = static_cast<uint8*>(TexPlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
				for(uint32 Y = 0; Y < ImageHeight; Y++ )
				{
					uint8* DestPtr = &MipData[(ImageHeight - 1 - Y) * ImageWidth * sizeof(FColor)];
					FColor* SrcPtr = const_cast<FColor*>(&Buffer[(ImageHeight - 1 - Y) * ImageWidth]);
					for(uint32 X = 0; X < ImageWidth; X++ )
					{
						*DestPtr++ = SrcPtr->B;
						*DestPtr++ = SrcPtr->G;
						*DestPtr++ = SrcPtr->R;
						*DestPtr++ = 0xFF;
						SrcPtr++;
					}
				}
				ScreenTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
				ScreenTexture->UpdateResource();		
			}
		}
	}
}


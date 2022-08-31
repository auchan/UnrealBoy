// License: See LICENSE.txt file


#include "UnrealBoyEmulatorActor.h"

#include "ImageUtils.h"
#include "UnrealBoyLCD.h"
#include "UnrealBoyMotherboard.h"


// Sets default values
AUnrealBoyEmulatorActor::AUnrealBoyEmulatorActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(USceneComponent::GetDefaultSceneRootVariableName());
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUnrealBoyEmulatorActor::BeginPlay()
{
	Super::BeginPlay();

	if (InputComponent)
	{
		// Key press
		InputComponent->BindAction(TEXT("Up"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnUpKeyPressed);
		InputComponent->BindAction(TEXT("Right"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnRightKeyPressed);
		InputComponent->BindAction(TEXT("Down"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnDownKeyPressed);
		InputComponent->BindAction(TEXT("Left"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnLeftKeyPressed);
		InputComponent->BindAction(TEXT("Function_A"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnFunctionAPressed);
		InputComponent->BindAction(TEXT("Function_B"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnFunctionBPressed);
		InputComponent->BindAction(TEXT("Select"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnSelectKeyPressed);
		InputComponent->BindAction(TEXT("Start"), IE_Pressed, this, &AUnrealBoyEmulatorActor::OnStartKeyPressed);

		// Key release
		InputComponent->BindAction(TEXT("Up"), IE_Released, this, &AUnrealBoyEmulatorActor::OnUpKeyReleased);
		InputComponent->BindAction(TEXT("Right"), IE_Released, this, &AUnrealBoyEmulatorActor::OnRightKeyReleased);
		InputComponent->BindAction(TEXT("Down"), IE_Released, this, &AUnrealBoyEmulatorActor::OnDownKeyReleased);
		InputComponent->BindAction(TEXT("Left"), IE_Released, this, &AUnrealBoyEmulatorActor::OnLeftKeyReleased);
		InputComponent->BindAction(TEXT("Function_A"), IE_Released, this, &AUnrealBoyEmulatorActor::OnFunctionAReleased);
		InputComponent->BindAction(TEXT("Function_B"), IE_Released, this, &AUnrealBoyEmulatorActor::OnFunctionBReleased);
		InputComponent->BindAction(TEXT("Select"), IE_Released, this, &AUnrealBoyEmulatorActor::OnSelectKeyReleased);
		InputComponent->BindAction(TEXT("Start"), IE_Released, this, &AUnrealBoyEmulatorActor::OnStartKeyReleased);
	}

	if (!CartridgeFilePath.FilePath.IsEmpty())
	{
		Emulator.Start(CartridgeFilePath.FilePath);
		Emulator.GetOnFrameDoneDelegate().AddUObject(this, &AUnrealBoyEmulatorActor::OnFrameDone);
	}
}

void AUnrealBoyEmulatorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Emulator.GetOnFrameDoneDelegate().RemoveAll(this);
	Emulator.Stop();
	Super::EndPlay(EndPlayReason);
}

void AUnrealBoyEmulatorActor::OnFrameDone()
{
	CaptureScreen();
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

void AUnrealBoyEmulatorActor::OnUpKeyPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Up, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnUpKeyReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Up, EUnrealBoyKeyEvent::Released);
}

void AUnrealBoyEmulatorActor::OnRightKeyPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Right, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnRightKeyReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Right, EUnrealBoyKeyEvent::Released);
}

void AUnrealBoyEmulatorActor::OnDownKeyPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Down, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnDownKeyReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Down, EUnrealBoyKeyEvent::Released);
}

void AUnrealBoyEmulatorActor::OnLeftKeyPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Left, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnLeftKeyReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Left, EUnrealBoyKeyEvent::Released);
}

void AUnrealBoyEmulatorActor::OnFunctionAPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::A, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnFunctionAReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::A, EUnrealBoyKeyEvent::Released);
}

void AUnrealBoyEmulatorActor::OnFunctionBPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::B, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnFunctionBReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::B, EUnrealBoyKeyEvent::Released);
}

void AUnrealBoyEmulatorActor::OnSelectKeyPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Select, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnSelectKeyReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Select, EUnrealBoyKeyEvent::Released);
}

void AUnrealBoyEmulatorActor::OnStartKeyPressed()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Start, EUnrealBoyKeyEvent::Pressed);
}

void AUnrealBoyEmulatorActor::OnStartKeyReleased()
{
	Emulator.OnKeyEvent(EUnrealBoyKeyType::Start, EUnrealBoyKeyEvent::Released);
}


// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealBoyEmulator.h"
#include "UnrealBoyEmulatorActor.generated.h"

UCLASS()
class UNREALBOY_API AUnrealBoyEmulatorActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnrealBoyEmulatorActor();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnFrameDone();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor)
	void CaptureScreen();
	
	void CopyImageBufferToTexture(uint32 ImageWidth, uint32 ImageHeight, const TArray<FColor>& Buffer, UTexture2D*& InScreenTexture);

protected:
	void OnUpKeyPressed();
	void OnUpKeyReleased();
	
	void OnRightKeyPressed();
	void OnRightKeyReleased();
	
	void OnDownKeyPressed();
	void OnDownKeyReleased();
	
	void OnLeftKeyPressed();
	void OnLeftKeyReleased();
	
	void OnFunctionAPressed();
	void OnFunctionAReleased();
	
	void OnFunctionBPressed();
	void OnFunctionBReleased();
	
	void OnSelectKeyPressed();
	void OnSelectKeyReleased();
	
	void OnStartKeyPressed();
	void OnStartKeyReleased();
	
private:
	UPROPERTY(EditAnywhere, meta = (RelativeToGameDir))
	FFilePath CartridgeFilePath;

	UPROPERTY(EditAnywhere)
	AActor* ScreenDisplayActor;
	
	UPROPERTY(EditAnywhere, Transient)
	UTexture2D* ScreenTexture;
	
	UPROPERTY(EditAnywhere, Transient)
	UTexture2D* TileMap1Texture;
	
	UPROPERTY(EditAnywhere, Transient)
	UTexture2D* TileMap2Texture;

	float AccumulatedTime = 0.f;
	
	FUnrealBoyEmulator Emulator;
};

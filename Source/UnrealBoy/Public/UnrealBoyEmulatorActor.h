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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	FString CartridgeFilePath;
	
	FUnrealBoyEmulator Emulator;
};

// License: See LICENSE.txt file


#include "UnrealBoyEmulatorActor.h"

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
	
	if (!CartridgeFilePath.IsEmpty())
	{
		Emulator.Start(CartridgeFilePath);
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
	Emulator.Tick(DeltaTime);
}


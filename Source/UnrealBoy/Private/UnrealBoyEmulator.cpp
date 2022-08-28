// License: See LICENSE.txt file


#include "UnrealBoyEmulator.h"

#include "UnrealBoyLog.h"
#include "UnrealBoyMotherboard.h"

int32 FUnrealBoyEmulator::Start(const FString& InROMFilePath)
{
	TArray<uint8> ROMData;
	if (!LoadROMFile(InROMFilePath, ROMData))
	{
		return -1;
	}
	
	Motherboard = MakeShareable(new FUnrealBoyMotherboard(ROMData));
	return 0;
}

void FUnrealBoyEmulator::Tick(float InDeltaTime)
{
	if (!IsValid())
	{
		return;
	}
	
	if (Motherboard)
	{
		Motherboard->Tick();
	}
}

int32 FUnrealBoyEmulator::Stop()
{
	Motherboard.Reset();
	return 0;
}

bool FUnrealBoyEmulator::IsValid() const
{
	return Motherboard.IsValid();
}

const TArray<FColor>& FUnrealBoyEmulator::GetScreenBuffer() const
{
	check(IsValid());
	return Motherboard->GetScreenBuffer();
}

const TArray<FColor>& FUnrealBoyEmulator::GetTileMap1Buffer() const
{
	return Motherboard->GetTileMap1Buffer();
}

bool FUnrealBoyEmulator::LoadROMFile(const FString& InROMFilePath, TArray<uint8>& OutLoadedData) const
{
	// Ensure file exist
	return FFileHelper::LoadFileToArray(OutLoadedData, *InROMFilePath);
}

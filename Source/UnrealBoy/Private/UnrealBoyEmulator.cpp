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

	uint16 Address = 0;
	// // LD BC,d16
	// Motherboard->WriteMemory(Address++, 0x01);
	// Motherboard->WriteMemory(Address++, 0x02);
	// Motherboard->WriteMemory(Address++, 0x03);
	//
	// // LD A,B
	// Motherboard->WriteMemory(Address++, 0x78);

	UE_LOG(LogUnrealBoy, Display, TEXT("State0 - %s"), *Motherboard->DumpState(""));
	for (int32 Count = 1;  Count <= 10; Count++)
	{
		Motherboard->Tick();
		UE_LOG(LogUnrealBoy, Display, TEXT("State%d - %s"), Count, *Motherboard->DumpState(""));
	}

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

bool FUnrealBoyEmulator::LoadROMFile(const FString& InROMFilePath, TArray<uint8>& OutLoadedData) const
{
	// Ensure file exist
	return FFileHelper::LoadFileToArray(OutLoadedData, *InROMFilePath);
}

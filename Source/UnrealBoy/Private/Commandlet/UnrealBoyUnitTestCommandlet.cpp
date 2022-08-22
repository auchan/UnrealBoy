// License: See LICENSE.txt file


#include "UnrealBoyUnitTestCommandlet.h"

#include "UnrealBoyEmulator.h"
#include "Interfaces/IPluginManager.h"

int32 UUnrealBoyUnitTestCommandlet::Main(const FString& Params)
{
	const FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("UnrealBoy"))->GetBaseDir();
	const FString TestROMFilePath = FPaths::Combine(PluginDir, TEXT("Resources/ROMs/cpu_instrs.gb"));
	
	FUnrealBoyEmulator Emulator;
	Emulator.Start(TestROMFilePath);
	// Tick loop
	for (int32 TickCount = 0; TickCount < 1000; TickCount++)
	{
		Emulator.Tick(0);
	}
	Emulator.Stop();
	return 0;
}

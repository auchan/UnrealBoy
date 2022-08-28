// License: See LICENSE.md file

#include "UnrealBoyTests.h"

#include "UnrealBoyEmulator.h"
#include "Interfaces/IPluginManager.h"

bool FUnrealBoyCPUInstructionTest::RunTest(const FString& Parameters)
{
	const FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("UnrealBoy"))->GetBaseDir();
	const FString TestROMFilePath = FPaths::Combine(PluginDir, TEXT("Resources/ROMs/cpu_instrs.gb"));
	
	FUnrealBoyEmulator Emulator;
	Emulator.Start(TestROMFilePath);
	// Tick loop
	for (int32 TickCount = 0; TickCount < 100; TickCount++)
	{
		Emulator.Tick(0);
	}
	Emulator.Stop();

	return true;
}
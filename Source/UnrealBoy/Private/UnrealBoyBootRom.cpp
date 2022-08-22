// License: See LICENSE.txt file


#include "UnrealBoyBootRom.h"

#include "UnrealBoyMotherboard.h"

FString FUnrealBoyBootRom::FastBotRomFilePath = TEXT("UnrealBoyFastBot");

FUnrealBoyBootRom::FUnrealBoyBootRom(FUnrealBoyMotherboard& InMotherboard, const FString& BootRomFilePath)
: Motherboard(InMotherboard)
{
	if (BootRomFilePath == FastBotRomFilePath)
	{
		InMotherboard.WriteMemories({
			// Set stack pointer SP = 0xFEFF
			{0x00, 0x31},
			{0x01, 0xFE},
			{0x02, 0xFF},

			//Inject jump to 0xFC00
			{0x03, 0xC3},
			{0x04, 0xFC},
			{0x05, 0x00},

			// Inject code to disable boot-ROM
			{0xFC, 0x3E},
			{0xFD, 0x01},
			{0xFE, 0xE0},
			{0xFF, 0x50},
		});
	}
}

FUnrealBoyBootRom::~FUnrealBoyBootRom()
{
}

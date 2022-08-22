// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

class FUnrealBoyMotherboard;
/**
 * 
 */
class UNREALBOY_API FUnrealBoyBootRom
{
public:
	FUnrealBoyBootRom(FUnrealBoyMotherboard& InMotherboard, const FString& BootRomFilePath = FastBotRomFilePath);
	~FUnrealBoyBootRom();

	static FString FastBotRomFilePath;
private:
	/** Reference to motherboard */
	FUnrealBoyMotherboard& Motherboard;
};

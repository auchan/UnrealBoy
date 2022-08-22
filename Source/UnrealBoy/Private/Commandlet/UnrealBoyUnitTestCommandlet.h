// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "UObject/Object.h"
#include "UnrealBoyUnitTestCommandlet.generated.h"

/**
 * 
 */
UCLASS()
class UNREALBOY_API UUnrealBoyUnitTestCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	virtual int32 Main(const FString& Params) override;
};

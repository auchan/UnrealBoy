// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealBoyModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

// License: See LICENSE.md file

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UObject/Object.h"
#include "UnrealBoyTypes.h"
#include "UnrealBoyEmulatorWidget.generated.h"

class SUnrealBoyEmulatorWidget;

/**
 * An UMG widget for emulator simulation
 */
UCLASS()
class UNREALBOY_API UUnrealBoyEmulatorWidget : public UWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cartridge, meta = (RelativeToGameDir))
	FFilePath CartridgeFilePath;

	UFUNCTION(BlueprintCallable, Category = "Unreal Boy")
	void OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent);

	UFUNCTION(BlueprintCallable, Category = "Unreal Boy")
	void SaveData();

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:

	TSharedPtr<SUnrealBoyEmulatorWidget> EmulatorWidget;
};

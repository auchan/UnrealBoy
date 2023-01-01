// License: See LICENSE.md file


#include "Widget/UnrealBoyEmulatorWidget.h"

#include "Widget/SUnrealBoyEmulatorWidget.h"

void UUnrealBoyEmulatorWidget::OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent)
{
	if (EmulatorWidget)
	{
		EmulatorWidget->OnKeyEvent(KeyType, KeyEvent);
	}
}

void UUnrealBoyEmulatorWidget::SaveData()
{
	if (EmulatorWidget)
	{
		EmulatorWidget->SaveData();
	}
}

void UUnrealBoyEmulatorWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	EmulatorWidget = nullptr;
}

TSharedRef<SWidget> UUnrealBoyEmulatorWidget::RebuildWidget()
{
	return SAssignNew(EmulatorWidget, SUnrealBoyEmulatorWidget)
		.CartridgeFilePath(CartridgeFilePath.FilePath);
}

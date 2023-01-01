// License: See LICENSE.md file


#include "Widget/SUnrealBoyEmulatorWidget.h"
#include "Widgets/Images/SImage.h"

#include "SlateOptMacros.h"
#include "UnrealBoyEmulator.h"
#include "Utility/UnrealBoyUtility.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SUnrealBoyEmulatorWidget::Construct(const FArguments& InArgs)
{
	CartridgeFilePath = InArgs._CartridgeFilePath;

	ChildSlot
	[
		SAssignNew(ScreenBufferImage, SImage)
		.Image(&ScreenBufferBrush)
	];

	Emulator = MakeShareable(new FUnrealBoyEmulator(EUnrealBoyEmulatorFlags::AutoLoadWhenStart));
	Emulator->Start(CartridgeFilePath);
	if (Emulator->IsValid())
	{
		Emulator->GetOnFrameDoneDelegate().AddSP(this, &SUnrealBoyEmulatorWidget::OnFrameDone);

		ScreenBufferTexture = FUnrealBoyUtility::CreateTextureForScreenBuffer(*Emulator);
		ScreenBufferBrush.ImageSize = FVector2D(ScreenBufferTexture->GetSizeX(), ScreenBufferTexture->GetSizeY());
		ScreenBufferBrush.SetResourceObject(ScreenBufferTexture);
	}
	else
	{
		Emulator.Reset();
		ScreenBufferBrush.SetResourceObject(nullptr);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SUnrealBoyEmulatorWidget::~SUnrealBoyEmulatorWidget()
{
	if (Emulator)
	{
		Emulator->Stop();
	}
}

void SUnrealBoyEmulatorWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!Emulator)
	{
		return;
	}

	Emulator->Tick(InDeltaTime);
}

void SUnrealBoyEmulatorWidget::OnFrameDone() const
{
	if (!Emulator)
	{
		return;
	}

	FUnrealBoyUtility::UpdateTextureForScreenBuffer(*Emulator, ScreenBufferTexture);
}

FReply SUnrealBoyEmulatorWidget::OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return OnKeyDown(MyGeometry, InKeyEvent);
}

FReply SUnrealBoyEmulatorWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return OnKeyEvent(InKeyEvent, true);
}

FReply SUnrealBoyEmulatorWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return OnKeyEvent(InKeyEvent, false);
}

void SUnrealBoyEmulatorWidget::OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent) const
{
	if (Emulator)
	{
		Emulator->OnKeyEvent(KeyType, KeyEvent);
	}
}

void SUnrealBoyEmulatorWidget::SaveData() const
{
	if (Emulator)
	{
		Emulator->Save();
	}
}

FReply SUnrealBoyEmulatorWidget::OnKeyEvent(const FKeyEvent& InKeyEvent, bool bKeyDown) const
{
	if (!Emulator)
	{
		return FReply::Unhandled();
	}

	const FKey Key = InKeyEvent.GetKey();
	const EUnrealBoyKeyEvent UnrealBoyKeyEvent = bKeyDown ? EUnrealBoyKeyEvent::Pressed : EUnrealBoyKeyEvent::Released;
	if (Key == EKeys::Up)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::Up, UnrealBoyKeyEvent);
	}
	else if (Key == EKeys::Down)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::Down, UnrealBoyKeyEvent);
	}
	else if (Key == EKeys::Left)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::Left, UnrealBoyKeyEvent);
	}
	else if (Key == EKeys::Right)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::Right, UnrealBoyKeyEvent);
	}
	else if (Key == EKeys::A || Key == EKeys::Z)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::A, UnrealBoyKeyEvent);
	}
	else if (Key == EKeys::B || Key == EKeys::X)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::B, UnrealBoyKeyEvent);
	}
	else if (Key == EKeys::Enter)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::Select, UnrealBoyKeyEvent);
	}
	else if (Key == EKeys::SpaceBar)
	{
		Emulator->OnKeyEvent(EUnrealBoyKeyType::Start, UnrealBoyKeyEvent);
	}
	else
	{
		return FReply::Unhandled();
	}
	return FReply::Handled();
}

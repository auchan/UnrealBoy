// License: See LICENSE.md file

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "UnrealBoyTypes.h"

class FUnrealBoyEmulator;
/**
 * 
 */
class UNREALBOY_API SUnrealBoyEmulatorWidget : public SCompoundWidget, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SUnrealBoyEmulatorWidget)
		{}
		SLATE_ARGUMENT(FString, CartridgeFilePath)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual ~SUnrealBoyEmulatorWidget() override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool SupportsKeyboardFocus() const override { return true; }

	// Begin FGCObject Interface
	void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;
	// End FGCObject Interface

	void OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent) const;

	void SaveData() const;

protected:
	void OnFrameDone() const;

	FReply OnKeyEvent(const FKeyEvent& InKeyEvent, bool bKeyDown) const;

protected:
	FString CartridgeFilePath;

	TSharedPtr<SImage> ScreenBufferImage;

	FSlateBrush ScreenBufferBrush;

	UTexture2D* ScreenBufferTexture = nullptr;

	TSharedPtr<FUnrealBoyEmulator> Emulator;

	float AccumulatedTime = 0.f;
};

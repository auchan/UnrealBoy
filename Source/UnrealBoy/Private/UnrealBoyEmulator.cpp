// License: See LICENSE.txt file


#include "UnrealBoyEmulator.h"

#include "UnrealBoyLog.h"
#include "UnrealBoyMotherboard.h"

int32 FUnrealBoyEmulator::Start(const FString& InROMFilePath)
{
	TArray<uint8> ROMData;
	if (!LoadROMFile(InROMFilePath, ROMData))
	{
		return -1;
	}
	
	Motherboard = MakeShareable(new FUnrealBoyMotherboard(ROMData));

	SaveDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), InROMFilePath + TEXT(".ubsave"));
	if (HasAnyFlags(EUnrealBoyEmulatorFlags::AutoLoadWhenStart))
	{
		LoadData(SaveDataFilePath);
	}

	return 0;
}

void FUnrealBoyEmulator::Tick(float InDeltaTime)
{
	if (!IsValid())
	{
		return;
	}
	
	if (Motherboard)
	{
		Motherboard->Tick();
	}
}

int32 FUnrealBoyEmulator::Stop()
{
	if (HasAnyFlags(EUnrealBoyEmulatorFlags::AutoSaveWhenStop))
	{
		SaveData(SaveDataFilePath);
	}

	Motherboard.Reset();
	return 0;
}

bool FUnrealBoyEmulator::IsValid() const
{
	return Motherboard.IsValid();
}

const TArray<FColor>& FUnrealBoyEmulator::GetScreenBuffer() const
{
	check(IsValid());
	return Motherboard->GetScreenBuffer();
}

const TArray<FColor>& FUnrealBoyEmulator::GetTileMap1Buffer() const
{
	return Motherboard->GetTileMap1Buffer();
}

FSimpleMulticastDelegate& FUnrealBoyEmulator::GetOnFrameDoneDelegate() const
{
	return Motherboard->GetOnFrameDoneDelegate();
}

void FUnrealBoyEmulator::OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent)
{
	if (!IsValid())
	{
		return;
	}
	Motherboard->OnKeyEvent(KeyType, KeyEvent);
}

bool FUnrealBoyEmulator::LoadROMFile(const FString& InROMFilePath, TArray<uint8>& OutLoadedData) const
{
	const FString ROMFileFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), InROMFilePath);
	// Ensure file exist
	return FFileHelper::LoadFileToArray(OutLoadedData, *ROMFileFullPath);
}

bool FUnrealBoyEmulator::HasAnyFlags(uint32 InFlags) const
{
	return (Flags & InFlags) != 0;
}

void FUnrealBoyEmulator::LoadData(const FString& InFilePath)
{
	const TUniquePtr<FArchive> Ar = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*InFilePath));
	if (!Ar || !Motherboard)
	{
		return;
	}

	// Read
	Serialize(*Ar.Get());
}

void FUnrealBoyEmulator::SaveData(const FString& InFilePath)
{
	const TUniquePtr<FArchive> Ar = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*InFilePath, FILEWRITE_AllowRead));
	if (!Ar || !Motherboard)
	{
		return;
	}

	// Write
	Serialize(*Ar.Get());
}

void FUnrealBoyEmulator::Serialize(FArchive& Ar)
{
	int32 CustomVer = 0;
	if (Ar.IsSaving())
	{
		CustomVer = static_cast<int32>(FUnrealBoySaveVersion::LatestVersion);
	}
	Ar << CustomVer;

	const FCustomVersion RegisteredVersion = FCurrentCustomVersions::Get(FUnrealBoySaveVersion::GUID).GetValue();
	Ar.SetCustomVersion(FUnrealBoySaveVersion::GUID, CustomVer, RegisteredVersion.GetFriendlyName());

	Motherboard->Serialize(Ar);
}

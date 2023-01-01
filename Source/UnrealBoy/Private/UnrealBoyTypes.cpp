// License: See LICENSE.txt file


#include "UnrealBoyTypes.h"

// Unique Unreal Boy Save version id
const FGuid FUnrealBoySaveVersion::GUID(0x75A90097, 0x8D5B2904, 0x388BEFA0, 0xD3872AA2);
// Register Unreal Boy Save version
FCustomVersionRegistration GRegisterUnrealBoySaveVersion(FUnrealBoySaveVersion::GUID, FUnrealBoySaveVersion::LatestVersion, TEXT("UnrealBoySaveVerion"));
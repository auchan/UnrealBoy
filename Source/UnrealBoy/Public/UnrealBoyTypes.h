// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

class FUnrealBoyMemoryUnit
{
public:
	FUnrealBoyMemoryUnit() : Address(0), Value(0) {}
	FUnrealBoyMemoryUnit(const uint16 InAddress, const uint8 InValue) : Address(InAddress), Value(InValue) {}
	
	uint16 Address;
	uint8 Value;
};

class IUnrealBoySerializable
{
public:
	virtual void Serialize(FArchive& Ar) = 0;

	virtual ~IUnrealBoySerializable() {}
};


namespace UnrealBoy
{
	constexpr uint8 INTR_VBLANK = (1 << 0);	
	constexpr uint8 INTR_LCDC = (1 << 1);	
	constexpr uint8 INTR_TIMER = (1 << 2);	
	constexpr uint8 INTR_SERIAL = (1 << 3);	
	constexpr uint8 INTR_HIGHTOLOW = (1 << 4);
}

namespace UnrealBoyAddressNames
{
	constexpr uint16 InterruptVBlank = 0x0040;
	constexpr uint16 InterruptLCDC = 0x0048;
	constexpr uint16 InterruptTimer = 0x0050;
	constexpr uint16 InterruptSerial = 0x0058;
	constexpr uint16 InterruptHighToLow = 0x0060;
	
	constexpr uint16 VRAM_BEGIN = 0x8000; // Video RAM
	constexpr uint16 VRAM_END = 0xA000;
	constexpr uint16 OAM_BEGIN = 0xFE00; // Object Attribute Memory (OAM)
	constexpr uint16 OAM_END = 0xFEA0;
	
	constexpr uint16 InterruptFlagRegister = 0xFF0F;
	constexpr uint16 InterruptEnableRegister = 0xFFFF;

	constexpr uint16 JoyPad = 0xFF00; // Joy Pad Register

	constexpr uint16 TimerDIV = 0xFF04; // Divider Register
	constexpr uint16 TIMA = 0xFF05; // Timer Counter
	constexpr uint16 TMA = 0xFF06; // Timer Modulo
	constexpr uint16 TAC = 0xFF07; // Timer Control
	
	constexpr uint16 LCDCRegister = 0xFF40; // LCD Control
	constexpr uint16 LCDStatRegister = 0xFF41; // LCD Status
	constexpr uint16 LCD_SCY = 0xFF42;	// Scroll Y
	constexpr uint16 LCD_SCX = 0xFF43; // Scroll X
	constexpr uint16 LCD_LY = 0xFF44; // Y Coordinate
	constexpr uint16 LCD_LYC = 0xFF45; // LY Compare
	constexpr uint16 DMA = 0xFF46;
	constexpr uint16 LCD_BGP = 0xFF47;
	constexpr uint16 LCD_OBP0 = 0xFF48;
	constexpr uint16 LCD_OBP1 = 0xFF49;
	constexpr uint16 LCD_WY = 0xFF4A; // Window Y position
	constexpr uint16 LCD_WX = 0xFF4B; // Window X position
}

UENUM(BlueprintType)
enum class EUnrealBoyKeyType : uint8
{
	Right,
	Left,
	Up,
	Down,
	A,
	B,
	Select,
	Start,
};

UENUM(BlueprintType)
enum class EUnrealBoyKeyEvent : uint8
{
	Pressed,
	Released ,
};

// Custom serialization version for changes made in Unreal Boy Emulator
struct FUnrealBoySaveVersion
{
	enum Type
	{
		// Before any version changes were made
		BeforeCustomVersionWasAdded = 0,

		// -----<new versions can be added above this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};

	// The GUID for this custom version number
	const static FGuid GUID;

private:
	FUnrealBoySaveVersion() {}
};
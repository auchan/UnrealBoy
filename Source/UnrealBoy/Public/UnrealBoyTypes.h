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

	// Audio registers
	constexpr uint16 NR10 = 0xFF10; // Channel 1 Sweep
	constexpr uint16 NR11 = 0xFF11; // Channel 1 Sound length/Wave pattern duty
	constexpr uint16 NR12 = 0xFF12; // Channel 1 Volume Envelope
	constexpr uint16 NR13 = 0xFF13; // Channel 1 Frequency lo
	constexpr uint16 NR14 = 0xFF14; // Channel 1 Frequency hi
	
	constexpr uint16 NR21 = 0xFF16; // Channel 2 Sound length/Wave pattern duty
	constexpr uint16 NR22 = 0xFF17; // Channel 2 Volume Envelope
	constexpr uint16 NR23 = 0xFF18; // Channel 2 Frequency lo
	constexpr uint16 NR24 = 0xFF19; // Channel 2 Frequency hi
	
	constexpr uint16 NR30 = 0xFF1A; // Channel 3 Sound on/off
	constexpr uint16 NR31 = 0xFF1B; // Channel 3 Sound length
	constexpr uint16 NR32 = 0xFF1C; // Channel 3 Select output level
	constexpr uint16 NR33 = 0xFF1D; // Channel 3 Frequency lo
	constexpr uint16 NR34 = 0xFF1E; // Channel 3 Frequency hi
	
	constexpr uint16 NR41 = 0xFF20; // Channel 4 Sound length
	constexpr uint16 NR42 = 0xFF21; // Channel 4 Volume Envelope
	constexpr uint16 NR43 = 0xFF22; // Channel 4 Polynomial counter
	constexpr uint16 NR44 = 0xFF23; // Channel 4 Counter/consecutive; Initial
	
	constexpr uint16 NR50 = 0xFF24; // Channel control / ON-OFF / Volume
	constexpr uint16 NR51 = 0xFF25; // Selection of Sound output terminal
	constexpr uint16 NR52 = 0xFF26; // Sound on/off
	
	// Wave RAM (Channel 3)
	constexpr uint16 WAVE_RAM_BEGIN = 0xFF30;
	constexpr uint16 WAVE_RAM_END = 0xFF40;
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
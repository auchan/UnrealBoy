// License: See LICENSE.md file


#include "UnrealBoyJoypad.h"
#include "UnrealBoyMotherboard.h"

namespace UnrealBoyNativeKeyType
{
	enum EType
	{
		P10,
		P11,
		P12,
		P13,
	};
};

FUnrealBoyJoyPad::FUnrealBoyJoyPad(FUnrealBoyMotherboard& InMotherboard)
	: Motherboard(InMotherboard)
	, JoyPadRegister(UNREALBOY_GET_MEMORY_REF(JoyPad))
{
	JoyPadRegister = 0xFF;
	PressedDirectionalKeys = 0xFF;
	PressedFunctionalKeys = 0xFF;
}

void FUnrealBoyJoyPad::WriteMemory(uint16 Address, uint8 Value)
{
	// Select Direction buttons (0=Select)
	const bool P14 = (Value >> 4) & 1;
	// Select Action buttons (0=Select)
	const bool P15 = (Value >> 5) & 1;

	// Make first 4 and last 2 bits true, while keeping selected bits
	Value = 0xFF & (Value | 0b11001111);
	if (P14 && P15)
	{
		// Not select any buttons
		return;	
	}
	else if (!P14 && !P15)
	{
		// Select both buttons, not allowed
		return;	
	}
	else if (!P14)
	{
		// Select direction button
		JoyPadRegister = Value & PressedDirectionalKeys;
	}
	else if (!P15)
	{
		// Select action button
		JoyPadRegister = Value & PressedFunctionalKeys;
	}
}

void FUnrealBoyJoyPad::OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent)
{
	bool bResetBit = (KeyEvent == EUnrealBoyKeyEvent::Pressed);
	
	switch (KeyType)
	{
	case EUnrealBoyKeyType::Right:
		SetKeyBit(PressedDirectionalKeys, UnrealBoyNativeKeyType::P10, bResetBit);
		break;
	case EUnrealBoyKeyType::Left:
		SetKeyBit(PressedDirectionalKeys, UnrealBoyNativeKeyType::P11, bResetBit);
		break;
	case EUnrealBoyKeyType::Up:
		SetKeyBit(PressedDirectionalKeys, UnrealBoyNativeKeyType::P12, bResetBit);
		break;
	case EUnrealBoyKeyType::Down:
		SetKeyBit(PressedDirectionalKeys, UnrealBoyNativeKeyType::P13, bResetBit);
		break;
	case EUnrealBoyKeyType::A:
		SetKeyBit(PressedFunctionalKeys, UnrealBoyNativeKeyType::P10, bResetBit);
		break;
	case EUnrealBoyKeyType::B:
		SetKeyBit(PressedFunctionalKeys, UnrealBoyNativeKeyType::P11, bResetBit);
		break;
	case EUnrealBoyKeyType::Select:
		SetKeyBit(PressedFunctionalKeys, UnrealBoyNativeKeyType::P12, bResetBit);
		break;
	case EUnrealBoyKeyType::Start:
		SetKeyBit(PressedFunctionalKeys, UnrealBoyNativeKeyType::P13, bResetBit);
		break;
	}
}

void FUnrealBoyJoyPad::SetKeyBit(uint8& Keys, uint8 Bit, bool bReset)
{
	if (bReset)
	{
		Keys &= ~(1 << Bit);
	}
	else
	{
		Keys |= (1 << Bit);
	}
}

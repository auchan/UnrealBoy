// License: See LICENSE.txt file


#include "UnrealBoyMotherboard.h"

#include "UnrealBoyCPU.h"
#include "UnrealBoyBootRom.h"
#include "UnrealBoyJoypad.h"
#include "UnrealBoyLCD.h"
#include "UnrealBoyTimer.h"
#include "Cartridge/UnrealBoyBaseMBC.h"
#include "Cartridge/UnrealBoyCartridge.h"


FUnrealBoyMotherboard::FUnrealBoyMotherboard(const TArray<uint8>& InROMData)
	: bBootRomEnabled(true)
{
	MemoryBlock.SetNumZeroed(1 << 16); // 2^16

	CPU = MakeShareable(new FUnrealBoyCPU(*this));
	BotRoom = MakeShareable(new FUnrealBoyBootRom(*this));
	MBC = FUnrealBoyCartridge::LoadROM(InROMData);
	check(MBC);
	LCD = MakeShareable(new FUnrealBoyLCD(*this));
	Timer = MakeShareable(new FUnrealBoyTimer(*this));
	JoyPad = MakeShareable(new FUnrealBoyJoyPad(*this));

	InitializeMemoryDelegates();
}

FUnrealBoyMotherboard::~FUnrealBoyMotherboard()
{
}

uint8 FUnrealBoyMotherboard::ReadMemory(uint16 Address)
{
	if (Address < 0x4000) // 16KB ROM bank 0
	{
		if (Address > 0xFF || !bBootRomEnabled)
		{
			return MBC->ReadMemory(Address);
		}
	}
	else if (Address < 0x8000) // 16KB switchable ROM bank
	{
		return MBC->ReadMemory(Address);
	}
	
	return MemoryBlock[Address];
}

void FUnrealBoyMotherboard::WriteMemory(uint16 Address, uint8 Value)
{
	bool bWrite = true;
	if (const FWriteMemoryDelegate* WriteMemoryDelegate = WriteMemoryDelegateMap.Find(Address))
	{
		if (WriteMemoryDelegate->IsBound())
		{
			bWrite = WriteMemoryDelegate->Execute(Address, MemoryBlock[Address], Value);
		}
	}

	if (!bWrite)
	{
		return;	
	}

	if (Address < 0x4000) // 16KB ROM bank #0
	{
		// Doesn't change the data. This is for MBC commands
		MBC->WriteMemory(Address, Value);
	}
	else if (0x4000 <= Address && Address < 0x8000) // 16KB switchable ROM bank
	{
		// Doesn't change the data. This is for MBC commands
		MBC->WriteMemory(Address, Value);
	}
	else if (0x8000 <= Address && Address < 0xA000) // 8KB Video RAM
	{
		MemoryBlock[Address] = Value;
		if (Address < 0x9800) // Is within tile data
		{
			// Mask out the byte of the tile (one tile has 16 bytes)
			LCD->AddChangedTile(Address & 0xFFF0);
		}
	}
	else if (0xFF00 <= Address && Address < 0xFF4C) // I/O ports
	{
		if (0xFF00 == Address)
		{
			JoyPad->WriteMemory(Address, Value);
		}
		else if (0xFF04 <= Address && Address < 0xFF08) // Timer
		{
			Timer->WriteMemory(Address, Value);
		}
		else if (Address == UnrealBoyAddressNames::LCDCRegister)
		{
			LCD->SetLCDC(Value);
		}
		else if (Address == UnrealBoyAddressNames::LCDStatRegister)
		{
			LCD->SetStat(Value);
		}
		else if (Address == UnrealBoyAddressNames::DMA)
		{
			TransferDMA(Value);
		}
		else
		{
			if (UnrealBoyAddressNames::LCD_BGP <= Address && Address <= UnrealBoyAddressNames::LCD_OBP1)
			{
				if (MemoryBlock[Address] != Value)
				{
					// Color palette changed, need clear cache
					LCD->RequestClearCache();
				}
			}
			
			MemoryBlock[Address] = Value;	
		}
	}
	else
	{
		MemoryBlock[Address] = Value;	
	}
}

void FUnrealBoyMotherboard::WriteMemories(std::initializer_list<FUnrealBoyMemoryUnit> MemoryUnits)
{
	for (const FUnrealBoyMemoryUnit& MemoryUnit : MemoryUnits)
	{
		MemoryBlock[MemoryUnit.Address] = MemoryUnit.Value;
	}
}

uint8& FUnrealBoyMotherboard::GetMemoryRef(uint16 Address)
{
	return MemoryBlock[Address];
}

TArrayView<uint8> FUnrealBoyMotherboard::GetMemoryRangeView(uint16 BeginAddress, uint16 EndAddress)
{
	return TArrayView<uint8>(&MemoryBlock[BeginAddress], EndAddress - BeginAddress);
}

const TArray<FColor>& FUnrealBoyMotherboard::GetScreenBuffer() const
{
	return LCD->GetScreenBuffer();
}

const TArray<FColor>& FUnrealBoyMotherboard::GetTileMap1Buffer() const
{
	return  LCD->GetTileMap1Buffer();
}

FSimpleMulticastDelegate& FUnrealBoyMotherboard::GetOnFrameDoneDelegate() const
{
	return LCD->OnFrameDone;
}

void FUnrealBoyMotherboard::OnKeyEvent(EUnrealBoyKeyType KeyType, EUnrealBoyKeyEvent KeyEvent)
{
	JoyPad->OnKeyEvent(KeyType, KeyEvent);
}

bool FUnrealBoyMotherboard::Tick()
{
	while (LCD->ProcessingFrame())
	{
		uint32 Cycles = CPU->Tick();

		if (CPU->IsHalted())
		{
			// Fast-forward to next interrupt:
			// As we are halted, we are guaranteed, that our state
			// cannot be altered by other factors than time.
			// For HighToLow interrupt it is indistinguishable whether
			// it gets triggered mid-frame or by next frame
			// Serial is not implemented, so this isn't a concern
			Cycles = FMath::Min(Timer->GetCyclesToInterrupt(), LCD->GetCyclesToInterrupt());
		}

		if (const uint8 TimerInterrupts = Timer->Tick(Cycles))
		{
			CPU->SetInterruptFlag(TimerInterrupts);
		}
		
		if (const uint8 LCDInterrupts = LCD->Tick(Cycles))
		{
			CPU->SetInterruptFlag(LCDInterrupts);
		}	
	}
	
	return true;
}

void FUnrealBoyMotherboard::TransferDMA(uint8 SrcAddress)
{
	constexpr uint16 DstAddress = 0xFE00;
	const uint16 SrcBaseAddress = SrcAddress * 0x100;
	for (uint16 i = 0; i < 0xA0; i++)
	{
		WriteMemory(DstAddress + i, ReadMemory(SrcBaseAddress + i));
	}
}

FString FUnrealBoyMotherboard::DumpState(const FString& InLabel) const
{
	FString State;
	State += FString("CPU: ") + CPU->DumpState(InLabel);
	return State;
}

void FUnrealBoyMotherboard::InitializeMemoryDelegates()
{
	WriteMemoryDelegateMap.Add(0xFF50).BindRaw(this, &FUnrealBoyMotherboard::OnDisableBootRom);
}

bool FUnrealBoyMotherboard::OnDisableBootRom(uint16 Address, uint8 OldValue, uint8 NewValue)
{
	if (!bBootRomEnabled)
	{
		// Already disabled
		return false;
	}
	
	if (NewValue == 1)
	{
		bBootRomEnabled = false;
	}

	// Disable write
	return false;
}

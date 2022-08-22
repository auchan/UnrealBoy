// License: See LICENSE.txt file


#include "UnrealBoyMotherboard.h"

#include "UnrealBoyCPU.h"
#include "UnrealBoyBootRom.h"
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

	InitializeMemoryDelegates();
}

FUnrealBoyMotherboard::~FUnrealBoyMotherboard()
{
}

uint8 FUnrealBoyMotherboard::ReadMemory(uint16 Address)
{
	if (Address < 0x4000) // 16KB ROM bank 0
	{
		if (Address <= 0xFF && bBootRomEnabled)
		{
			// TODO: Access boot rom, fix later
			// Time to sleep = =
		}
		else
		{
			return MBC->ReadMemory(Address);
		}
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

	if (bWrite)
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

bool FUnrealBoyMotherboard::Tick()
{
	CPU->Tick();
	return true;
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

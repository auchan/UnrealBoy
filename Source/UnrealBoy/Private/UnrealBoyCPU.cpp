// License: See LICENSE.txt file


#include "UnrealBoyCPU.h"

#include <sstream>
#include <bitset>

#include "UnrealBoyLog.h"
#include "UnrealBoyMotherboard.h"

#define UNREALBOY_DEBUG_CPU 0

namespace UnrealBoy
{
	using FInterruptRegInfo = TTuple<uint8, uint16>;
	TArray<FInterruptRegInfo> InterruptVector {
		FInterruptRegInfo {INTR_VBLANK, UnrealBoyAddressNames::InterruptVBlank},
		FInterruptRegInfo {INTR_LCDC, UnrealBoyAddressNames::InterruptLCDC},
		FInterruptRegInfo {INTR_TIMER, UnrealBoyAddressNames::InterruptTimer},
		FInterruptRegInfo {INTR_SERIAL, UnrealBoyAddressNames::InterruptSerial},
		FInterruptRegInfo {INTR_HIGHTOLOW, UnrealBoyAddressNames::InterruptHighToLow},
	};
	
	template<typename T>
	static FString IntegerToBinaryString(const T& X)
	{
		std::stringstream ss;
		ss << std::bitset<sizeof(T) * 8>(X);
		return UTF8_TO_TCHAR(ss.str().data());
	}
}

using namespace UnrealBoy;

FUnrealBoyCPU::FUnrealBoyCPU(FUnrealBoyMotherboard& InMotherboard)
	: A(0), F(0)
	, B(0), C(0)
	, D(0), E(0)
	, HL(0)
	, SP(0)
	, PC(0)
	, Motherboard(InMotherboard)
	, Opcodes(*this)
	, bInterruptQueued(false)
	, bInterruptMasterEnabled(false)
	, InterruptFlagRegister(InMotherboard.GetMemoryRef(UnrealBoyAddressNames::InterruptFlagRegister))
	, InterruptEnableRegister(InMotherboard.GetMemoryRef(UnrealBoyAddressNames::InterruptEnableRegister))
	, bHalted(false)
	, bStuck(false)
	, bStopped(false)
{
#if UNREALBOY_DEBUG_CPU
	FileHandle = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*(FPaths::ProjectSavedDir() / TEXT("UnrealBoy_CPU_Dump.txt")), FILEWRITE_AllowRead));
#endif
}

FUnrealBoyCPU::~FUnrealBoyCPU()
{
}

void FUnrealBoyCPU::Serialize(FArchive& Ar)
{
	//Ar.UsingCustomVersion()
	Ar << A << F;
	Ar << B << C;
	Ar << D << E;
	Ar << HL;
	Ar << SP;
	Ar << PC;
	Ar << bInterruptQueued;
	Ar << bInterruptMasterEnabled;
	Ar << bHalted;
	Ar << bStuck;
	Ar << bStopped;
}

uint8 FUnrealBoyCPU::Tick()
{
	if (CheckInterrupts())
	{
		bHalted = false;
		// TODO: return with the cycles it took to handle the interrupt
		return 0;
	}

	if (bHalted && bInterruptQueued)
	{
		bHalted = false;
		PC++;
	}
	else if (bHalted)
	{
		// TODO: Number of cycles for a HALT in effect?
		return 4;
	}

	const uint16 OldPC = PC;
	const uint8 ElapsedClockCycles = FetchAndExecute();
	if (!bHalted && OldPC == PC && !bStuck)
	{
		UE_LOG(LogUnrealBoy, Error, TEXT("CPU is stuck: %s"), *DumpState(""));
		bStuck = true;
	}
	bInterruptQueued = false;
	return ElapsedClockCycles;
}

uint8 FUnrealBoyCPU::FetchAndExecute()
{
	uint16 Opcode = Motherboard.ReadMemory(PC);
	uint8 PCExtra = 0;
	if (Opcode == 0xCB)	// Extension code
	{
		Opcode = Motherboard.ReadMemory(PC + 1);
		Opcode |= 0x100;
		PCExtra = 1;
	}

#if UNREALBOY_DEBUG_CPU
	if (FileHandle)
	{
		uint8 Operand1 = Motherboard.ReadMemory(PC + PCExtra + 1);
		FString Str = FString::Format(TEXT("{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11}\n"), {Opcode, PC + PCExtra, SP, A, F, B, C, D, E, HL >> 8, HL & 0xFF, Operand1});
		FTCHARToUTF8 UTF8String(GetData(Str), Str.Len());
		FileHandle->Serialize((UTF8CHAR*)UTF8String.Get(), UTF8String.Length() * sizeof(UTF8CHAR));
	}
#endif
	
	return Opcodes.ExecuteOpcode(Opcode);
}

bool FUnrealBoyCPU::CheckInterrupts()
{
	if (bInterruptQueued)
	{
		// Interrupt already queued. This happens only when using a debugger.
		return false;
	}

	if ((InterruptFlagRegister & 0b11111) & (InterruptEnableRegister & 0b11111))
	{
		for (const UnrealBoy::FInterruptRegInfo& RegInfo : UnrealBoy::InterruptVector)
		{
			if (HandleInterrupt(RegInfo.Key, RegInfo.Value))
			{
				bInterruptQueued = true;
				break;
			}
		}
		ensureAlwaysMsgf(bInterruptQueued, TEXT("No interrupt triggered, but it should!"));
		return true;
	}

	bInterruptQueued = false;
	return false;
}

bool FUnrealBoyCPU::HandleInterrupt(uint8 Flag, uint16 Address)
{
	if ((InterruptFlagRegister & Flag) & (InterruptEnableRegister & Flag))
	{
		// Clear interrupt flag
		if (bHalted)
		{
			// Escape HALT on return
			PC += 1;
		}

		// Handle interrupt vectors
		if (bInterruptMasterEnabled)
		{
			InterruptFlagRegister ^= Flag;	// Remove flag
			Motherboard.WriteMemory(SP - 1, PC >> 8);	// Write PC high address
			Motherboard.WriteMemory(SP - 2, PC & 0xFF);	// Write PC low address
			SP -= 2;
			PC = Address;
			bInterruptMasterEnabled = false;
		}

		return true;
	}

	return false;
}

void FUnrealBoyCPU::SetInterruptFlag(uint8 Flag)
{
	InterruptFlagRegister |= Flag;
}

void FUnrealBoyCPU::SetRegisterBC(uint16 X)
{
	B = X >> 8;
	C = X & 0xFF;
}

void FUnrealBoyCPU::SetRegisterDE(uint16 X)
{
	D = X >> 8;
	E = X & 0xFF;
}

bool FUnrealBoyCPU::GetFlagC() const
{
	return (F & (1 << FLAG_C)) != 0;
}

bool FUnrealBoyCPU::GetFlagH() const
{
	return (F & (1 << FLAG_H)) != 0;
}

bool FUnrealBoyCPU::GetFlagN() const
{
	return (F & (1 << FLAG_N)) != 0;
}

bool FUnrealBoyCPU::GetFlagZ() const
{
	return (F & (1 << FLAG_Z)) != 0;
}

bool FUnrealBoyCPU::GetFlagNC() const
{
	return (F & (1 << FLAG_C)) == 0;
}

bool FUnrealBoyCPU::GetFlagNZ() const
{
	return (F & (1 << FLAG_Z)) == 0;
}

bool FUnrealBoyCPU::IsHalted() const
{
	return bHalted;
}

FString FUnrealBoyCPU::DumpState(const FString& InLabel)
{
	uint8 Opcode = Motherboard.ReadMemory(PC);
	uint8 Opcode_1 = Motherboard.ReadMemory(PC + 1);
	uint8 Opcode_2 = Motherboard.ReadMemory(PC + 2);
	FString OpcodeString;
	if (Opcode == 0xCB)
	{
		OpcodeString = FString::Printf(TEXT("Opcode: 0x0%2X, 0x%02X, %s"), Opcode, Opcode_1, *Opcodes.GetCommandName(Opcode_1 + 0x100));
	}
	else
	{
		OpcodeString = FString::Printf(TEXT("Opcode: 0x0%2X, %s"), Opcode, *Opcodes.GetCommandName(Opcode));
	}

	const FStringFormatNamedArguments Args = {
		{TEXT("A"), A},
		{TEXT("F"), F},
		{TEXT("B"), B},
		{TEXT("C"), C},
		{TEXT("D"), D},
		{TEXT("E"), E},
		{TEXT("HL"), HL},
		{TEXT("SP"), SP},
		{TEXT("PC"), PC},
		{TEXT("Label"), InLabel},
		{TEXT("OpcodeString"), OpcodeString},
		{TEXT("IME"), OpcodeString},
		{TEXT("IE"), InterruptEnableRegister},
		{TEXT("IF"), InterruptFlagRegister},
		{TEXT("Halted"), bHalted},
		{TEXT("InterruptQueued"), bInterruptQueued},
		{TEXT("Stopped"), bStopped},
	};
	return FString::Printf(TEXT(
		"\n"
		"A: 0x%02X, F: 0x%02X, B: 0x%02X, "
		"C: 0x%02X, D: 0x%02X, E: 0x%02X, "
		"HL: 0x%04X, SP: 0x%04X, PC: 0x%04X ({%s})\n"
		"{%s}\n"
		"Interrupts - IME: {IME}, "
		"IE: 0b%s, "
		"IF: 0b%s\n"
		"halted:%d, "
		"interrupt_queued:%d, "
		"stopped:%d\n"
		), A, F, B,
			C, D, E,
			HL, SP, PC, *InLabel,
			*OpcodeString,
			*IntegerToBinaryString(InterruptEnableRegister),
			*IntegerToBinaryString(InterruptFlagRegister),
			bHalted,
			bInterruptQueued,
			bStopped
		);
}

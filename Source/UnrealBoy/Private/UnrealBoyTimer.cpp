// License: See LICENSE.md file


#include "UnrealBoyTimer.h"
#include "UnrealBoyMotherboard.h"

FUnrealBoyTimer::FUnrealBoyTimer(FUnrealBoyMotherboard& InMotherboard)
	: Motherboard(InMotherboard)
	, DIV(UNREALBOY_GET_MEMORY_REF(TimerDIV))
	, TIMA(UNREALBOY_GET_MEMORY_REF(TIMA))
	, TMA(UNREALBOY_GET_MEMORY_REF(TMA))
	, TAC(UNREALBOY_GET_MEMORY_REF(TAC))
	, DIVCounter(0)
	, TIMACounter(0)
	, Dividers({1024, 16, 64, 256})
{
}

void FUnrealBoyTimer::Serialize(FArchive& Ar)
{
	Ar << DIVCounter;
	Ar << TIMACounter;
}

uint8 FUnrealBoyTimer::ReadMemory(uint16 Address)
{
	// Not used
	check(false);
	return 0;
}

void FUnrealBoyTimer::WriteMemory(uint16 Address, uint8 Value)
{
	switch (Address)
	{
		case UnrealBoyAddressNames::TimerDIV:
			Reset();
			break;
		case UnrealBoyAddressNames::TIMA:
			TIMA = Value;
			break;
		case UnrealBoyAddressNames::TMA:
			TMA = Value;
			break;
		case UnrealBoyAddressNames::TAC:
			TAC = Value & 0b111;
			break;
		default:
			checkf(false, TEXT("Unexpected address 0x%04X"), Address);
			break;
	}
}

uint8 FUnrealBoyTimer::Tick(uint32 Cycles)
{
	DIVCounter += Cycles;
	// Add overflow bits to DIV
	DIV += (DIVCounter >> 8);
	// Remove the overflow bits
	DIVCounter &= 0xFF;

	if ((TAC & 0b100) == 0)
	{
		// Timer is disabled
		return 0;
	}

	TIMACounter += Cycles;
	const uint8 Divider = Dividers[TAC & 0b11];

	if (TIMACounter >= Divider)
	{
		TIMACounter -= Divider;
		const uint16 NextTIMA = TIMA + 1;
		if (NextTIMA > 0xFF)
		{
			TIMA = TMA;
			return UnrealBoy::INTR_TIMER;
		}
		else
		{
			TIMA = static_cast<uint8>(NextTIMA);
		}
	}

	return 0;
}

uint32 FUnrealBoyTimer::GetCyclesToInterrupt() const
{
	if ((TAC & 0b100) == 0)
	{
		// Timer is disabled
		// Large enough to ignore this
		return 1 << 16;
	}

	const uint8 Divider = Dividers[TAC & 0b11];
	const uint32 CyclesLeft = (static_cast<uint32>(0x100 - TIMA) * Divider) - TIMACounter;
	return CyclesLeft;
}

void FUnrealBoyTimer::Reset()
{
	DIV = 0;
	DIVCounter = 0;
	TIMACounter = 0;
}

// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyOpcodes.h"
#include "UnrealBoyTypes.h"

class FUnrealBoyMotherboard;

namespace UnrealBoy
{
	uint8 FLAG_C = 4;
	uint8 FLAG_H = 5;
	uint8 FLAG_N = 6;
	uint8 FLAG_Z = 7;
}

/**
 * 
 */
class UNREALBOY_API FUnrealBoyCPU : public IUnrealBoySerializable
{
public:
	FUnrealBoyCPU(FUnrealBoyMotherboard& InMotherboard);
	virtual ~FUnrealBoyCPU() override;

	virtual void Serialize(FArchive& Ar) override;

	/**
	 * Make one tick on the emulated CPU
	 * @return the consumed clock cycles
	 */
	uint8 Tick();

	/**
	* Fetch one opcode pointed by PC and execute it
	* @return the consumed clock cycles
	*/
	uint8 FetchAndExecute();

	/**
	 * Check enabled interrupt and handle it
	 * @return whether interrupt occurs
	 */
	bool CheckInterrupts();

	/** Handle interrupt by jump to specified address */
	bool HandleInterrupt(uint8 Flag, uint16 Address);

	/** Set interrupt flag to register */
	void SetInterruptFlag(uint8 Flag);

	void SetRegisterBC(uint16 X);
	void SetRegisterDE(uint16 X);
	bool GetFlagC() const;
	bool GetFlagH() const;
	bool GetFlagN() const;
	bool GetFlagZ() const;
	bool GetFlagNC() const;
	bool GetFlagNZ() const;
	bool IsHalted() const;

	FString DumpState(const FString& InLabel);

	friend class FUnrealBoyOpcodes;
	
private:
	/** Registers */
	uint8 A, /** acc/arg */		F; /** flags */
	uint8 B,					C;
	uint8 D,					E;
	uint16 HL; /** address */
	uint16 SP; /** stack pointer */
	uint16 PC; /** program counter */

	/** Reference to motherboard */
	FUnrealBoyMotherboard& Motherboard;

	/** Opcodes */
	FUnrealBoyOpcodes Opcodes;

	/** Interrupts */
	bool bInterruptQueued;
	bool bInterruptMasterEnabled;
	uint8& InterruptFlagRegister;
	uint8& InterruptEnableRegister;

	/** State */
	bool bHalted;
	bool bStuck;
	bool bStopped;

	TUniquePtr<FArchive> FileHandle = nullptr;
};

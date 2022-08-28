// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"
#include "UnrealBoyBaseMBC.h"

/**
* MBC type 1, has two banking modes:
* 0: 2MB ROM and 8KB RAM
* 1: 512KB ROM and 32KB RAM
*/
class FUnrealBoyMBC1 : public FUnrealBoyBaseMBC
{
public:
	FUnrealBoyMBC1() = default;
	
	virtual uint8 ReadMemory(uint16 Address) override;

	virtual void WriteMemory(uint16 Address, uint8 Value) override;

protected:
	/** ROM bank number register */
	uint8 BankSelectRegister1 = 1;
	
	/** RAM bank number or upper bits of ROM bank number register */
	uint8 BankSelectRegister2 = 0;

	/** Whether RAM bank is enabled */ 
	bool bRamBankEnabled = false;

	/** Banking mode: 0 or 1 */
	uint8 BankingMode = 0;
};

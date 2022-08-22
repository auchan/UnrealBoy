﻿// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

class FUnrealBoyCPU;
/**
 * 
 */
class UNREALBOY_API FUnrealBoyOpcodes
{
	DECLARE_DELEGATE_RetVal(uint8, FOpcodeFunction)
public:
	FUnrealBoyOpcodes(FUnrealBoyCPU& InCPU);

	void InitOpcodeTable();
	uint8 ExecuteOpcode(uint16 Opcode);
	uint8 GetOpcodeLength(uint16 Opcode);
	const FOpcodeFunction& GetOpcodeFunction();
	FString GetCommandName(uint16 Opcode);

private:
	// Op functions
	uint8 NOP_00() const;
	uint8 LD_01() const;
	uint8 LD_02() const;
	uint8 INC_03() const;
	uint8 INC_04() const;
	uint8 DEC_05() const;
	uint8 LD_06() const;
	uint8 RLCA_07() const;
	uint8 LD_08() const;
	uint8 ADD_09() const;
	uint8 LD_0A() const;
	uint8 DEC_0B() const;
	uint8 INC_0C() const;
	uint8 DEC_0D() const;
	uint8 LD_0E() const;
	uint8 RRCA_0F() const;
	uint8 STOP_10() const;
	uint8 LD_11() const;
	uint8 LD_12() const;
	uint8 INC_13() const;
	uint8 INC_14() const;
	uint8 DEC_15() const;
	uint8 LD_16() const;
	uint8 RLA_17() const;
	uint8 JR_18() const;
	uint8 ADD_19() const;
	uint8 LD_1A() const;
	uint8 DEC_1B() const;
	uint8 INC_1C() const;
	uint8 DEC_1D() const;
	uint8 LD_1E() const;
	uint8 RRA_1F() const;
	uint8 JR_20() const;
	uint8 LD_21() const;
	uint8 LD_22() const;
	uint8 INC_23() const;
	uint8 INC_24() const;
	uint8 DEC_25() const;
	uint8 LD_26() const;
	uint8 DAA_27() const;
	uint8 JR_28() const;
	uint8 ADD_29() const;
	uint8 LD_2A() const;
	uint8 DEC_2B() const;
	uint8 INC_2C() const;
	uint8 DEC_2D() const;
	uint8 LD_2E() const;
	uint8 CPL_2F() const;
	uint8 JR_30() const;
	uint8 LD_31() const;
	uint8 LD_32() const;
	uint8 INC_33() const;
	uint8 INC_34() const;
	uint8 DEC_35() const;
	uint8 LD_36() const;
	uint8 SCF_37() const;
	uint8 JR_38() const;
	uint8 ADD_39() const;
	uint8 LD_3A() const;
	uint8 DEC_3B() const;
	uint8 INC_3C() const;
	uint8 DEC_3D() const;
	uint8 LD_3E() const;
	uint8 CCF_3F() const;
	uint8 LD_40() const;
	uint8 LD_41() const;
	uint8 LD_42() const;
	uint8 LD_43() const;
	uint8 LD_44() const;
	uint8 LD_45() const;
	uint8 LD_46() const;
	uint8 LD_47() const;
	uint8 LD_48() const;
	uint8 LD_49() const;
	uint8 LD_4A() const;
	uint8 LD_4B() const;
	uint8 LD_4C() const;
	uint8 LD_4D() const;
	uint8 LD_4E() const;
	uint8 LD_4F() const;
	uint8 LD_50() const;
	uint8 LD_51() const;
	uint8 LD_52() const;
	uint8 LD_53() const;
	uint8 LD_54() const;
	uint8 LD_55() const;
	uint8 LD_56() const;
	uint8 LD_57() const;
	uint8 LD_58() const;
	uint8 LD_59() const;
	uint8 LD_5A() const;
	uint8 LD_5B() const;
	uint8 LD_5C() const;
	uint8 LD_5D() const;
	uint8 LD_5E() const;
	uint8 LD_5F() const;
	uint8 LD_60() const;
	uint8 LD_61() const;
	uint8 LD_62() const;
	uint8 LD_63() const;
	uint8 LD_64() const;
	uint8 LD_65() const;
	uint8 LD_66() const;
	uint8 LD_67() const;
	uint8 LD_68() const;
	uint8 LD_69() const;
	uint8 LD_6A() const;
	uint8 LD_6B() const;
	uint8 LD_6C() const;
	uint8 LD_6D() const;
	uint8 LD_6E() const;
	uint8 LD_6F() const;
	uint8 LD_70() const;
	uint8 LD_71() const;
	uint8 LD_72() const;
	uint8 LD_73() const;
	uint8 LD_74() const;
	uint8 LD_75() const;
	uint8 HALT_76() const;
	uint8 LD_77() const;
	uint8 LD_78() const;
	uint8 LD_79() const;
	uint8 LD_7A() const;
	uint8 LD_7B() const;
	uint8 LD_7C() const;
	uint8 LD_7D() const;
	uint8 LD_7E() const;
	uint8 LD_7F() const;
	uint8 ADD_80() const;
	uint8 ADD_81() const;
	uint8 ADD_82() const;
	uint8 ADD_83() const;
	uint8 ADD_84() const;
	uint8 ADD_85() const;
	uint8 ADD_86() const;
	uint8 ADD_87() const;
	uint8 ADC_88() const;
	uint8 ADC_89() const;
	uint8 ADC_8A() const;
	uint8 ADC_8B() const;
	uint8 ADC_8C() const;
	uint8 ADC_8D() const;
	uint8 ADC_8E() const;
	uint8 ADC_8F() const;
	uint8 SUB_90() const;
	uint8 SUB_91() const;
	uint8 SUB_92() const;
	uint8 SUB_93() const;
	uint8 SUB_94() const;
	uint8 SUB_95() const;
	uint8 SUB_96() const;
	uint8 SUB_97() const;
	uint8 SBC_98() const;
	uint8 SBC_99() const;
	uint8 SBC_9A() const;
	uint8 SBC_9B() const;
	uint8 SBC_9C() const;
	uint8 SBC_9D() const;
	uint8 SBC_9E() const;
	uint8 SBC_9F() const;
	uint8 AND_A0() const;
	uint8 AND_A1() const;
	uint8 AND_A2() const;
	uint8 AND_A3() const;
	uint8 AND_A4() const;
	uint8 AND_A5() const;
	uint8 AND_A6() const;
	uint8 AND_A7() const;
	uint8 XOR_A8() const;
	uint8 XOR_A9() const;
	uint8 XOR_AA() const;
	uint8 XOR_AB() const;
	uint8 XOR_AC() const;
	uint8 XOR_AD() const;
	uint8 XOR_AE() const;
	uint8 XOR_AF() const;
	uint8 OR_B0() const;
	uint8 OR_B1() const;
	uint8 OR_B2() const;
	uint8 OR_B3() const;
	uint8 OR_B4() const;
	uint8 OR_B5() const;
	uint8 OR_B6() const;
	uint8 OR_B7() const;
	uint8 CP_B8() const;
	uint8 CP_B9() const;
	uint8 CP_BA() const;
	uint8 CP_BB() const;
	uint8 CP_BC() const;
	uint8 CP_BD() const;
	uint8 CP_BE() const;
	uint8 CP_BF() const;
	uint8 RET_C0() const;
	uint8 POP_C1() const;
	uint8 JP_C2() const;
	uint8 JP_C3() const;
	uint8 CALL_C4() const;
	uint8 PUSH_C5() const;
	uint8 ADD_C6() const;
	uint8 RST_C7() const;
	uint8 RET_C8() const;
	uint8 RET_C9() const;
	uint8 JP_CA() const;
	uint8 PREFIX_CB() const;
	uint8 CALL_CC() const;
	uint8 CALL_CD() const;
	uint8 ADC_CE() const;
	uint8 RST_CF() const;
	uint8 RET_D0() const;
	uint8 POP_D1() const;
	uint8 JP_D2() const;
	uint8 CALL_D4() const;
	uint8 PUSH_D5() const;
	uint8 SUB_D6() const;
	uint8 RST_D7() const;
	uint8 RET_D8() const;
	uint8 RETI_D9() const;
	uint8 JP_DA() const;
	uint8 CALL_DC() const;
	uint8 SBC_DE() const;
	uint8 RST_DF() const;
	uint8 LDH_E0() const;
	uint8 POP_E1() const;
	uint8 LD_E2() const;
	uint8 PUSH_E5() const;
	uint8 AND_E6() const;
	uint8 RST_E7() const;
	uint8 ADD_E8() const;
	uint8 JP_E9() const;
	uint8 LD_EA() const;
	uint8 XOR_EE() const;
	uint8 RST_EF() const;
	uint8 LDH_F0() const;
	uint8 POP_F1() const;
	uint8 LD_F2() const;
	uint8 DI_F3() const;
	uint8 PUSH_F5() const;
	uint8 OR_F6() const;
	uint8 RST_F7() const;
	uint8 LD_F8() const;
	uint8 LD_F9() const;
	uint8 LD_FA() const;
	uint8 EI_FB() const;
	uint8 CP_FE() const;
	uint8 RST_FF() const;
	uint8 RLC_100() const;
	uint8 RLC_101() const;
	uint8 RLC_102() const;
	uint8 RLC_103() const;
	uint8 RLC_104() const;
	uint8 RLC_105() const;
	uint8 RLC_106() const;
	uint8 RLC_107() const;
	uint8 RRC_108() const;
	uint8 RRC_109() const;
	uint8 RRC_10A() const;
	uint8 RRC_10B() const;
	uint8 RRC_10C() const;
	uint8 RRC_10D() const;
	uint8 RRC_10E() const;
	uint8 RRC_10F() const;
	uint8 RL_110() const;
	uint8 RL_111() const;
	uint8 RL_112() const;
	uint8 RL_113() const;
	uint8 RL_114() const;
	uint8 RL_115() const;
	uint8 RL_116() const;
	uint8 RL_117() const;
	uint8 RR_118() const;
	uint8 RR_119() const;
	uint8 RR_11A() const;
	uint8 RR_11B() const;
	uint8 RR_11C() const;
	uint8 RR_11D() const;
	uint8 RR_11E() const;
	uint8 RR_11F() const;
	uint8 SLA_120() const;
	uint8 SLA_121() const;
	uint8 SLA_122() const;
	uint8 SLA_123() const;
	uint8 SLA_124() const;
	uint8 SLA_125() const;
	uint8 SLA_126() const;
	uint8 SLA_127() const;
	uint8 SRA_128() const;
	uint8 SRA_129() const;
	uint8 SRA_12A() const;
	uint8 SRA_12B() const;
	uint8 SRA_12C() const;
	uint8 SRA_12D() const;
	uint8 SRA_12E() const;
	uint8 SRA_12F() const;
	uint8 SWAP_130() const;
	uint8 SWAP_131() const;
	uint8 SWAP_132() const;
	uint8 SWAP_133() const;
	uint8 SWAP_134() const;
	uint8 SWAP_135() const;
	uint8 SWAP_136() const;
	uint8 SWAP_137() const;
	uint8 SRL_138() const;
	uint8 SRL_139() const;
	uint8 SRL_13A() const;
	uint8 SRL_13B() const;
	uint8 SRL_13C() const;
	uint8 SRL_13D() const;
	uint8 SRL_13E() const;
	uint8 SRL_13F() const;
	uint8 BIT_140() const;
	uint8 BIT_141() const;
	uint8 BIT_142() const;
	uint8 BIT_143() const;
	uint8 BIT_144() const;
	uint8 BIT_145() const;
	uint8 BIT_146() const;
	uint8 BIT_147() const;
	uint8 BIT_148() const;
	uint8 BIT_149() const;
	uint8 BIT_14A() const;
	uint8 BIT_14B() const;
	uint8 BIT_14C() const;
	uint8 BIT_14D() const;
	uint8 BIT_14E() const;
	uint8 BIT_14F() const;
	uint8 BIT_150() const;
	uint8 BIT_151() const;
	uint8 BIT_152() const;
	uint8 BIT_153() const;
	uint8 BIT_154() const;
	uint8 BIT_155() const;
	uint8 BIT_156() const;
	uint8 BIT_157() const;
	uint8 BIT_158() const;
	uint8 BIT_159() const;
	uint8 BIT_15A() const;
	uint8 BIT_15B() const;
	uint8 BIT_15C() const;
	uint8 BIT_15D() const;
	uint8 BIT_15E() const;
	uint8 BIT_15F() const;
	uint8 BIT_160() const;
	uint8 BIT_161() const;
	uint8 BIT_162() const;
	uint8 BIT_163() const;
	uint8 BIT_164() const;
	uint8 BIT_165() const;
	uint8 BIT_166() const;
	uint8 BIT_167() const;
	uint8 BIT_168() const;
	uint8 BIT_169() const;
	uint8 BIT_16A() const;
	uint8 BIT_16B() const;
	uint8 BIT_16C() const;
	uint8 BIT_16D() const;
	uint8 BIT_16E() const;
	uint8 BIT_16F() const;
	uint8 BIT_170() const;
	uint8 BIT_171() const;
	uint8 BIT_172() const;
	uint8 BIT_173() const;
	uint8 BIT_174() const;
	uint8 BIT_175() const;
	uint8 BIT_176() const;
	uint8 BIT_177() const;
	uint8 BIT_178() const;
	uint8 BIT_179() const;
	uint8 BIT_17A() const;
	uint8 BIT_17B() const;
	uint8 BIT_17C() const;
	uint8 BIT_17D() const;
	uint8 BIT_17E() const;
	uint8 BIT_17F() const;
	uint8 RES_180() const;
	uint8 RES_181() const;
	uint8 RES_182() const;
	uint8 RES_183() const;
	uint8 RES_184() const;
	uint8 RES_185() const;
	uint8 RES_186() const;
	uint8 RES_187() const;
	uint8 RES_188() const;
	uint8 RES_189() const;
	uint8 RES_18A() const;
	uint8 RES_18B() const;
	uint8 RES_18C() const;
	uint8 RES_18D() const;
	uint8 RES_18E() const;
	uint8 RES_18F() const;
	uint8 RES_190() const;
	uint8 RES_191() const;
	uint8 RES_192() const;
	uint8 RES_193() const;
	uint8 RES_194() const;
	uint8 RES_195() const;
	uint8 RES_196() const;
	uint8 RES_197() const;
	uint8 RES_198() const;
	uint8 RES_199() const;
	uint8 RES_19A() const;
	uint8 RES_19B() const;
	uint8 RES_19C() const;
	uint8 RES_19D() const;
	uint8 RES_19E() const;
	uint8 RES_19F() const;
	uint8 RES_1A0() const;
	uint8 RES_1A1() const;
	uint8 RES_1A2() const;
	uint8 RES_1A3() const;
	uint8 RES_1A4() const;
	uint8 RES_1A5() const;
	uint8 RES_1A6() const;
	uint8 RES_1A7() const;
	uint8 RES_1A8() const;
	uint8 RES_1A9() const;
	uint8 RES_1AA() const;
	uint8 RES_1AB() const;
	uint8 RES_1AC() const;
	uint8 RES_1AD() const;
	uint8 RES_1AE() const;
	uint8 RES_1AF() const;
	uint8 RES_1B0() const;
	uint8 RES_1B1() const;
	uint8 RES_1B2() const;
	uint8 RES_1B3() const;
	uint8 RES_1B4() const;
	uint8 RES_1B5() const;
	uint8 RES_1B6() const;
	uint8 RES_1B7() const;
	uint8 RES_1B8() const;
	uint8 RES_1B9() const;
	uint8 RES_1BA() const;
	uint8 RES_1BB() const;
	uint8 RES_1BC() const;
	uint8 RES_1BD() const;
	uint8 RES_1BE() const;
	uint8 RES_1BF() const;
	uint8 SET_1C0() const;
	uint8 SET_1C1() const;
	uint8 SET_1C2() const;
	uint8 SET_1C3() const;
	uint8 SET_1C4() const;
	uint8 SET_1C5() const;
	uint8 SET_1C6() const;
	uint8 SET_1C7() const;
	uint8 SET_1C8() const;
	uint8 SET_1C9() const;
	uint8 SET_1CA() const;
	uint8 SET_1CB() const;
	uint8 SET_1CC() const;
	uint8 SET_1CD() const;
	uint8 SET_1CE() const;
	uint8 SET_1CF() const;
	uint8 SET_1D0() const;
	uint8 SET_1D1() const;
	uint8 SET_1D2() const;
	uint8 SET_1D3() const;
	uint8 SET_1D4() const;
	uint8 SET_1D5() const;
	uint8 SET_1D6() const;
	uint8 SET_1D7() const;
	uint8 SET_1D8() const;
	uint8 SET_1D9() const;
	uint8 SET_1DA() const;
	uint8 SET_1DB() const;
	uint8 SET_1DC() const;
	uint8 SET_1DD() const;
	uint8 SET_1DE() const;
	uint8 SET_1DF() const;
	uint8 SET_1E0() const;
	uint8 SET_1E1() const;
	uint8 SET_1E2() const;
	uint8 SET_1E3() const;
	uint8 SET_1E4() const;
	uint8 SET_1E5() const;
	uint8 SET_1E6() const;
	uint8 SET_1E7() const;
	uint8 SET_1E8() const;
	uint8 SET_1E9() const;
	uint8 SET_1EA() const;
	uint8 SET_1EB() const;
	uint8 SET_1EC() const;
	uint8 SET_1ED() const;
	uint8 SET_1EE() const;
	uint8 SET_1EF() const;
	uint8 SET_1F0() const;
	uint8 SET_1F1() const;
	uint8 SET_1F2() const;
	uint8 SET_1F3() const;
	uint8 SET_1F4() const;
	uint8 SET_1F5() const;
	uint8 SET_1F6() const;
	uint8 SET_1F7() const;
	uint8 SET_1F8() const;
	uint8 SET_1F9() const;
	uint8 SET_1FA() const;
	uint8 SET_1FB() const;
	uint8 SET_1FC() const;
	uint8 SET_1FD() const;
	uint8 SET_1FE() const;
	uint8 SET_1FF() const;
	uint8 NoOpcode();

private:
	FUnrealBoyCPU& CPU;
	uint16 Value;
	
	TArray<FOpcodeFunction> OpcodeTable;
};

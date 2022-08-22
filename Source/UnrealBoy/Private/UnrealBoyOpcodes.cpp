
/*
* THIS FILE IS AUTO-GENERATED!!!
* DO NOT MODIFY THIS FILE.
* CHANGES TO THE CODE SHOULD BE MADE IN 'opcodes_gen.py'.
*/


#include "UnrealBoyOpcodes.h"

#include "UnrealBoyCPU.h"
#include "UnrealBoyMotherboard.h"

using namespace UnrealBoy;

FUnrealBoyOpcodes::FUnrealBoyOpcodes(FUnrealBoyCPU& InCPU)
	: CPU(InCPU)
	, Value(0)
{
    InitOpcodeTable();
}

uint8 FUnrealBoyOpcodes::NOP_00() const // 00 NOP
{
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_01() const // 01 LD BC,d16
{
    CPU.SetRegisterBC(Value);
    CPU.PC += 3;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_02() const // 02 LD (BC),A
{
    CPU.Motherboard.WriteMemory(((CPU.B << 8) + CPU.C), CPU.A);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_03() const // 03 INC BC
{
    auto Temp = ((CPU.B << 8) + CPU.C) + 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.SetRegisterBC(Temp);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_04() const // 04 INC B
{
    auto Temp = CPU.B + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.B & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::DEC_05() const // 05 DEC B
{
    auto Temp = CPU.B - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.B & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_06() const // 06 LD B,d8
{
    CPU.B = Value;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLCA_07() const // 07 RLCA
{
    uint16 Temp = (CPU.A << 1) + (CPU.A >> 7);
    uint8 Flag = 0b00000000;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_08() const // 08 LD (a16),SP
{
    CPU.Motherboard.WriteMemory(Value, CPU.SP & 0xFF);
    CPU.Motherboard.WriteMemory(Value+1, CPU.SP >> 8);
    CPU.PC += 3;
    return 20;
}

uint8 FUnrealBoyOpcodes::ADD_09() const // 09 ADD HL,BC
{
    auto Temp = CPU.HL + ((CPU.B << 8) + CPU.C);
    uint8 Flag = 0b00000000;
    Flag += (((CPU.HL & 0xFFF) + (((CPU.B << 8) + CPU.C) & 0xFFF)) > 0xFFF) << FLAG_H;
    Flag += (Temp > 0xFFFF) << FLAG_C;
    CPU.F &= 0b10000000;
    CPU.F |= Flag;
    Temp &= 0xFFFF;
    CPU.HL = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_0A() const // 0A LD A,(BC)
{
    CPU.A = CPU.Motherboard.ReadMemory(((CPU.B << 8) + CPU.C));
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::DEC_0B() const // 0B DEC BC
{
    auto Temp = ((CPU.B << 8) + CPU.C) - 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.SetRegisterBC(Temp);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_0C() const // 0C INC C
{
    auto Temp = CPU.C + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.C & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::DEC_0D() const // 0D DEC C
{
    auto Temp = CPU.C - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.C & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_0E() const // 0E LD C,d8
{
    CPU.C = Value;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRCA_0F() const // 0F RRCA
{
    uint16 Temp = (CPU.A >> 1) + ((CPU.A & 1) << 7) + ((CPU.A & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::STOP_10() const // 10 STOP 0
{
    CPU.PC += 2;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_11() const // 11 LD DE,d16
{
    CPU.SetRegisterDE(Value);
    CPU.PC += 3;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_12() const // 12 LD (DE),A
{
    CPU.Motherboard.WriteMemory(((CPU.D << 8) + CPU.E), CPU.A);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_13() const // 13 INC DE
{
    auto Temp = ((CPU.D << 8) + CPU.E) + 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.SetRegisterDE(Temp);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_14() const // 14 INC D
{
    auto Temp = CPU.D + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.D & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::DEC_15() const // 15 DEC D
{
    auto Temp = CPU.D - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.D & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_16() const // 16 LD D,d8
{
    CPU.D = Value;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLA_17() const // 17 RLA
{
    uint16 Temp = (CPU.A << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::JR_18() const // 18 JR r8
{
    CPU.PC += 2 + ((Value ^ 0x80) - 0x80);
    CPU.PC &= 0xFFFF;
    return 12;
}

uint8 FUnrealBoyOpcodes::ADD_19() const // 19 ADD HL,DE
{
    auto Temp = CPU.HL + ((CPU.D << 8) + CPU.E);
    uint8 Flag = 0b00000000;
    Flag += (((CPU.HL & 0xFFF) + (((CPU.D << 8) + CPU.E) & 0xFFF)) > 0xFFF) << FLAG_H;
    Flag += (Temp > 0xFFFF) << FLAG_C;
    CPU.F &= 0b10000000;
    CPU.F |= Flag;
    Temp &= 0xFFFF;
    CPU.HL = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_1A() const // 1A LD A,(DE)
{
    CPU.A = CPU.Motherboard.ReadMemory(((CPU.D << 8) + CPU.E));
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::DEC_1B() const // 1B DEC DE
{
    auto Temp = ((CPU.D << 8) + CPU.E) - 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.SetRegisterDE(Temp);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_1C() const // 1C INC E
{
    auto Temp = CPU.E + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.E & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::DEC_1D() const // 1D DEC E
{
    auto Temp = CPU.E - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.E & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_1E() const // 1E LD E,d8
{
    CPU.E = Value;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRA_1F() const // 1F RRA
{
    uint16 Temp = (CPU.A >> 1) + (CPU.GetFlagC() << 7) + ((CPU.A & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::JR_20() const // 20 JR NZ,r8
{
    CPU.PC += 2;
    if (CPU.GetFlagNZ())
    {
        CPU.PC += ((Value ^ 0x80) - 0x80);
        CPU.PC &= 0xFFFF;
        return 12;
    }
    else
    {
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::LD_21() const // 21 LD HL,d16
{
    CPU.HL = Value;
    CPU.PC += 3;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_22() const // 22 LD (HL+),A
{
    CPU.Motherboard.WriteMemory(CPU.HL, CPU.A);
    CPU.HL += 1;
    CPU.HL &= 0xFFFF;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_23() const // 23 INC HL
{
    auto Temp = CPU.HL + 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.HL = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_24() const // 24 INC H
{
    auto Temp = (CPU.HL >> 8) + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += ((((CPU.HL >> 8) & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::DEC_25() const // 25 DEC H
{
    auto Temp = (CPU.HL >> 8) - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += ((((CPU.HL >> 8) & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_26() const // 26 LD H,d8
{
    CPU.HL = (CPU.HL & 0x00FF) | (Value << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::DAA_27() const // 27 DAA
{
    uint8 Temp = CPU.A;
    uint8 Corr = 0;
    Corr |= CPU.GetFlagH() ? 0x06 : 0x00;
    Corr |= CPU.GetFlagC() ? 0x60 : 0x00;
    if (CPU.GetFlagN())
    {
        Temp -= Corr;
    }
    else
    {
        Corr |= (Temp & 0x0F) > 0x09 ? 0x06 : 0x00;
        Corr |= Temp > 0x99 ? 0x60 : 0x00;
        Temp += Corr;
    }
    uint8 Flag = 0;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += ((Corr & 0x60) != 0) << FLAG_C;
    CPU.F &= 0b01000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::JR_28() const // 28 JR Z,r8
{
    CPU.PC += 2;
    if (CPU.GetFlagZ())
    {
        CPU.PC += ((Value ^ 0x80) - 0x80);
        CPU.PC &= 0xFFFF;
        return 12;
    }
    else
    {
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::ADD_29() const // 29 ADD HL,HL
{
    auto Temp = CPU.HL + CPU.HL;
    uint8 Flag = 0b00000000;
    Flag += (((CPU.HL & 0xFFF) + (CPU.HL & 0xFFF)) > 0xFFF) << FLAG_H;
    Flag += (Temp > 0xFFFF) << FLAG_C;
    CPU.F &= 0b10000000;
    CPU.F |= Flag;
    Temp &= 0xFFFF;
    CPU.HL = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_2A() const // 2A LD A,(HL+)
{
    CPU.A = CPU.Motherboard.ReadMemory(CPU.HL);
    CPU.HL += 1;
    CPU.HL &= 0xFFFF;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::DEC_2B() const // 2B DEC HL
{
    auto Temp = CPU.HL - 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.HL = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_2C() const // 2C INC L
{
    auto Temp = (CPU.HL & 0xFF) + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += ((((CPU.HL & 0xFF) & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::DEC_2D() const // 2D DEC L
{
    auto Temp = (CPU.HL & 0xFF) - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += ((((CPU.HL & 0xFF) & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_2E() const // 2E LD L,d8
{
    CPU.HL = (CPU.HL & 0xFF00) | (Value & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::CPL_2F() const // 2F CPL
{
    CPU.A = (~CPU.A) & 0xFF;
    uint8 Flag = 0b01100000;
    CPU.F &= 0b10010000;
    CPU.F |= Flag;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::JR_30() const // 30 JR NC,r8
{
    CPU.PC += 2;
    if (CPU.GetFlagNC())
    {
        CPU.PC += ((Value ^ 0x80) - 0x80);
        CPU.PC &= 0xFFFF;
        return 12;
    }
    else
    {
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::LD_31() const // 31 LD SP,d16
{
    CPU.SP = Value;
    CPU.PC += 3;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_32() const // 32 LD (HL-),A
{
    CPU.Motherboard.WriteMemory(CPU.HL, CPU.A);
    CPU.HL -= 1;
    CPU.HL &= 0xFFFF;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_33() const // 33 INC SP
{
    auto Temp = CPU.SP + 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.SP = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_34() const // 34 INC (HL)
{
    auto Temp = CPU.Motherboard.ReadMemory(CPU.HL) + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.Motherboard.ReadMemory(CPU.HL) & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 1;
    return 12;
}

uint8 FUnrealBoyOpcodes::DEC_35() const // 35 DEC (HL)
{
    auto Temp = CPU.Motherboard.ReadMemory(CPU.HL) - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.Motherboard.ReadMemory(CPU.HL) & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 1;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_36() const // 36 LD (HL),d8
{
    CPU.Motherboard.WriteMemory(CPU.HL, Value);
    CPU.PC += 2;
    return 12;
}

uint8 FUnrealBoyOpcodes::SCF_37() const // 37 SCF
{
    uint8 Flag = 0b00010000;
    CPU.F &= 0b10000000;
    CPU.F |= Flag;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::JR_38() const // 38 JR C,r8
{
    CPU.PC += 2;
    if (CPU.GetFlagC())
    {
        CPU.PC += ((Value ^ 0x80) - 0x80);
        CPU.PC &= 0xFFFF;
        return 12;
    }
    else
    {
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::ADD_39() const // 39 ADD HL,SP
{
    auto Temp = CPU.HL + CPU.SP;
    uint8 Flag = 0b00000000;
    Flag += (((CPU.HL & 0xFFF) + (CPU.SP & 0xFFF)) > 0xFFF) << FLAG_H;
    Flag += (Temp > 0xFFFF) << FLAG_C;
    CPU.F &= 0b10000000;
    CPU.F |= Flag;
    Temp &= 0xFFFF;
    CPU.HL = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_3A() const // 3A LD A,(HL-)
{
    CPU.A = CPU.Motherboard.ReadMemory(CPU.HL);
    CPU.HL -= 1;
    CPU.HL &= 0xFFFF;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::DEC_3B() const // 3B DEC SP
{
    auto Temp = CPU.SP - 1;
    // No flag operations;
    Temp &= 0xFFFF;
    CPU.SP = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::INC_3C() const // 3C INC A
{
    auto Temp = CPU.A + 1;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (1 & 0xF)) > 0xF) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::DEC_3D() const // 3D DEC A
{
    auto Temp = CPU.A - 1;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (1 & 0xF)) < 0) << FLAG_H;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_3E() const // 3E LD A,d8
{
    CPU.A = Value;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::CCF_3F() const // 3F CCF
{
    uint8 Flag = (CPU.F & 0b00010000) ^ 0b00010000;
    CPU.F &= 0b10000000;
    CPU.F |= Flag;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_40() const // 40 LD B,B
{
    CPU.B = CPU.B;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_41() const // 41 LD B,C
{
    CPU.B = CPU.C;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_42() const // 42 LD B,D
{
    CPU.B = CPU.D;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_43() const // 43 LD B,E
{
    CPU.B = CPU.E;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_44() const // 44 LD B,H
{
    CPU.B = (CPU.HL >> 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_45() const // 45 LD B,L
{
    CPU.B = (CPU.HL & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_46() const // 46 LD B,(HL)
{
    CPU.B = CPU.Motherboard.ReadMemory(CPU.HL);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_47() const // 47 LD B,A
{
    CPU.B = CPU.A;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_48() const // 48 LD C,B
{
    CPU.C = CPU.B;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_49() const // 49 LD C,C
{
    CPU.C = CPU.C;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_4A() const // 4A LD C,D
{
    CPU.C = CPU.D;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_4B() const // 4B LD C,E
{
    CPU.C = CPU.E;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_4C() const // 4C LD C,H
{
    CPU.C = (CPU.HL >> 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_4D() const // 4D LD C,L
{
    CPU.C = (CPU.HL & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_4E() const // 4E LD C,(HL)
{
    CPU.C = CPU.Motherboard.ReadMemory(CPU.HL);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_4F() const // 4F LD C,A
{
    CPU.C = CPU.A;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_50() const // 50 LD D,B
{
    CPU.D = CPU.B;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_51() const // 51 LD D,C
{
    CPU.D = CPU.C;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_52() const // 52 LD D,D
{
    CPU.D = CPU.D;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_53() const // 53 LD D,E
{
    CPU.D = CPU.E;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_54() const // 54 LD D,H
{
    CPU.D = (CPU.HL >> 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_55() const // 55 LD D,L
{
    CPU.D = (CPU.HL & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_56() const // 56 LD D,(HL)
{
    CPU.D = CPU.Motherboard.ReadMemory(CPU.HL);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_57() const // 57 LD D,A
{
    CPU.D = CPU.A;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_58() const // 58 LD E,B
{
    CPU.E = CPU.B;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_59() const // 59 LD E,C
{
    CPU.E = CPU.C;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_5A() const // 5A LD E,D
{
    CPU.E = CPU.D;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_5B() const // 5B LD E,E
{
    CPU.E = CPU.E;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_5C() const // 5C LD E,H
{
    CPU.E = (CPU.HL >> 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_5D() const // 5D LD E,L
{
    CPU.E = (CPU.HL & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_5E() const // 5E LD E,(HL)
{
    CPU.E = CPU.Motherboard.ReadMemory(CPU.HL);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_5F() const // 5F LD E,A
{
    CPU.E = CPU.A;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_60() const // 60 LD H,B
{
    CPU.HL = (CPU.HL & 0x00FF) | (CPU.B << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_61() const // 61 LD H,C
{
    CPU.HL = (CPU.HL & 0x00FF) | (CPU.C << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_62() const // 62 LD H,D
{
    CPU.HL = (CPU.HL & 0x00FF) | (CPU.D << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_63() const // 63 LD H,E
{
    CPU.HL = (CPU.HL & 0x00FF) | (CPU.E << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_64() const // 64 LD H,H
{
    CPU.HL = (CPU.HL & 0x00FF) | ((CPU.HL >> 8) << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_65() const // 65 LD H,L
{
    CPU.HL = (CPU.HL & 0x00FF) | ((CPU.HL & 0xFF) << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_66() const // 66 LD H,(HL)
{
    CPU.HL = (CPU.HL & 0x00FF) | (CPU.Motherboard.ReadMemory(CPU.HL) << 8);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_67() const // 67 LD H,A
{
    CPU.HL = (CPU.HL & 0x00FF) | (CPU.A << 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_68() const // 68 LD L,B
{
    CPU.HL = (CPU.HL & 0xFF00) | (CPU.B & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_69() const // 69 LD L,C
{
    CPU.HL = (CPU.HL & 0xFF00) | (CPU.C & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_6A() const // 6A LD L,D
{
    CPU.HL = (CPU.HL & 0xFF00) | (CPU.D & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_6B() const // 6B LD L,E
{
    CPU.HL = (CPU.HL & 0xFF00) | (CPU.E & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_6C() const // 6C LD L,H
{
    CPU.HL = (CPU.HL & 0xFF00) | ((CPU.HL >> 8) & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_6D() const // 6D LD L,L
{
    CPU.HL = (CPU.HL & 0xFF00) | ((CPU.HL & 0xFF) & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_6E() const // 6E LD L,(HL)
{
    CPU.HL = (CPU.HL & 0xFF00) | (CPU.Motherboard.ReadMemory(CPU.HL) & 0xFF);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_6F() const // 6F LD L,A
{
    CPU.HL = (CPU.HL & 0xFF00) | (CPU.A & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_70() const // 70 LD (HL),B
{
    CPU.Motherboard.WriteMemory(CPU.HL, CPU.B);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_71() const // 71 LD (HL),C
{
    CPU.Motherboard.WriteMemory(CPU.HL, CPU.C);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_72() const // 72 LD (HL),D
{
    CPU.Motherboard.WriteMemory(CPU.HL, CPU.D);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_73() const // 73 LD (HL),E
{
    CPU.Motherboard.WriteMemory(CPU.HL, CPU.E);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_74() const // 74 LD (HL),H
{
    CPU.Motherboard.WriteMemory(CPU.HL, (CPU.HL >> 8));
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_75() const // 75 LD (HL),L
{
    CPU.Motherboard.WriteMemory(CPU.HL, (CPU.HL & 0xFF));
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::HALT_76() const // 76 HALT
{
    CPU.bHalted = true;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_77() const // 77 LD (HL),A
{
    CPU.Motherboard.WriteMemory(CPU.HL, CPU.A);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_78() const // 78 LD A,B
{
    CPU.A = CPU.B;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_79() const // 79 LD A,C
{
    CPU.A = CPU.C;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_7A() const // 7A LD A,D
{
    CPU.A = CPU.D;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_7B() const // 7B LD A,E
{
    CPU.A = CPU.E;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_7C() const // 7C LD A,H
{
    CPU.A = (CPU.HL >> 8);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_7D() const // 7D LD A,L
{
    CPU.A = (CPU.HL & 0xFF);
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_7E() const // 7E LD A,(HL)
{
    CPU.A = CPU.Motherboard.ReadMemory(CPU.HL);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_7F() const // 7F LD A,A
{
    CPU.A = CPU.A;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADD_80() const // 80 ADD A,B
{
    auto Temp = CPU.A + CPU.B;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.B & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADD_81() const // 81 ADD A,C
{
    auto Temp = CPU.A + CPU.C;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.C & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADD_82() const // 82 ADD A,D
{
    auto Temp = CPU.A + CPU.D;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.D & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADD_83() const // 83 ADD A,E
{
    auto Temp = CPU.A + CPU.E;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.E & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADD_84() const // 84 ADD A,H
{
    auto Temp = CPU.A + (CPU.HL >> 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + ((CPU.HL >> 8) & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADD_85() const // 85 ADD A,L
{
    auto Temp = CPU.A + (CPU.HL & 0xFF);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + ((CPU.HL & 0xFF) & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADD_86() const // 86 ADD A,(HL)
{
    auto Temp = CPU.A + CPU.Motherboard.ReadMemory(CPU.HL);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.Motherboard.ReadMemory(CPU.HL) & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::ADD_87() const // 87 ADD A,A
{
    auto Temp = CPU.A + CPU.A;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.A & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADC_88() const // 88 ADC A,B
{
    auto Temp = CPU.A + CPU.B + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.B & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADC_89() const // 89 ADC A,C
{
    auto Temp = CPU.A + CPU.C + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.C & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADC_8A() const // 8A ADC A,D
{
    auto Temp = CPU.A + CPU.D + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.D & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADC_8B() const // 8B ADC A,E
{
    auto Temp = CPU.A + CPU.E + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.E & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADC_8C() const // 8C ADC A,H
{
    auto Temp = CPU.A + (CPU.HL >> 8) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + ((CPU.HL >> 8) & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADC_8D() const // 8D ADC A,L
{
    auto Temp = CPU.A + (CPU.HL & 0xFF) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + ((CPU.HL & 0xFF) & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::ADC_8E() const // 8E ADC A,(HL)
{
    auto Temp = CPU.A + CPU.Motherboard.ReadMemory(CPU.HL) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.Motherboard.ReadMemory(CPU.HL) & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::ADC_8F() const // 8F ADC A,A
{
    auto Temp = CPU.A + CPU.A + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (CPU.A & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SUB_90() const // 90 SUB B
{
    auto Temp = CPU.A - CPU.B;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.B & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SUB_91() const // 91 SUB C
{
    auto Temp = CPU.A - CPU.C;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.C & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SUB_92() const // 92 SUB D
{
    auto Temp = CPU.A - CPU.D;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.D & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SUB_93() const // 93 SUB E
{
    auto Temp = CPU.A - CPU.E;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.E & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SUB_94() const // 94 SUB H
{
    auto Temp = CPU.A - (CPU.HL >> 8);
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - ((CPU.HL >> 8) & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SUB_95() const // 95 SUB L
{
    auto Temp = CPU.A - (CPU.HL & 0xFF);
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - ((CPU.HL & 0xFF) & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SUB_96() const // 96 SUB (HL)
{
    auto Temp = CPU.A - CPU.Motherboard.ReadMemory(CPU.HL);
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.Motherboard.ReadMemory(CPU.HL) & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::SUB_97() const // 97 SUB A
{
    auto Temp = CPU.A - CPU.A;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.A & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SBC_98() const // 98 SBC A,B
{
    auto Temp = CPU.A - CPU.B - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.B & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SBC_99() const // 99 SBC A,C
{
    auto Temp = CPU.A - CPU.C - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.C & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SBC_9A() const // 9A SBC A,D
{
    auto Temp = CPU.A - CPU.D - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.D & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SBC_9B() const // 9B SBC A,E
{
    auto Temp = CPU.A - CPU.E - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.E & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SBC_9C() const // 9C SBC A,H
{
    auto Temp = CPU.A - (CPU.HL >> 8) - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - ((CPU.HL >> 8) & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SBC_9D() const // 9D SBC A,L
{
    auto Temp = CPU.A - (CPU.HL & 0xFF) - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - ((CPU.HL & 0xFF) & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::SBC_9E() const // 9E SBC A,(HL)
{
    auto Temp = CPU.A - CPU.Motherboard.ReadMemory(CPU.HL) - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.Motherboard.ReadMemory(CPU.HL) & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::SBC_9F() const // 9F SBC A,A
{
    auto Temp = CPU.A - CPU.A - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.A & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::AND_A0() const // A0 AND B
{
    auto Temp = CPU.A & CPU.B;
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::AND_A1() const // A1 AND C
{
    auto Temp = CPU.A & CPU.C;
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::AND_A2() const // A2 AND D
{
    auto Temp = CPU.A & CPU.D;
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::AND_A3() const // A3 AND E
{
    auto Temp = CPU.A & CPU.E;
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::AND_A4() const // A4 AND H
{
    auto Temp = CPU.A & (CPU.HL >> 8);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::AND_A5() const // A5 AND L
{
    auto Temp = CPU.A & (CPU.HL & 0xFF);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::AND_A6() const // A6 AND (HL)
{
    auto Temp = CPU.A & CPU.Motherboard.ReadMemory(CPU.HL);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::AND_A7() const // A7 AND A
{
    auto Temp = CPU.A & CPU.A;
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::XOR_A8() const // A8 XOR B
{
    auto Temp = CPU.A ^ CPU.B;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::XOR_A9() const // A9 XOR C
{
    auto Temp = CPU.A ^ CPU.C;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::XOR_AA() const // AA XOR D
{
    auto Temp = CPU.A ^ CPU.D;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::XOR_AB() const // AB XOR E
{
    auto Temp = CPU.A ^ CPU.E;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::XOR_AC() const // AC XOR H
{
    auto Temp = CPU.A ^ (CPU.HL >> 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::XOR_AD() const // AD XOR L
{
    auto Temp = CPU.A ^ (CPU.HL & 0xFF);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::XOR_AE() const // AE XOR (HL)
{
    auto Temp = CPU.A ^ CPU.Motherboard.ReadMemory(CPU.HL);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::XOR_AF() const // AF XOR A
{
    auto Temp = CPU.A ^ CPU.A;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::OR_B0() const // B0 OR B
{
    auto Temp = CPU.A | CPU.B;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::OR_B1() const // B1 OR C
{
    auto Temp = CPU.A | CPU.C;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::OR_B2() const // B2 OR D
{
    auto Temp = CPU.A | CPU.D;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::OR_B3() const // B3 OR E
{
    auto Temp = CPU.A | CPU.E;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::OR_B4() const // B4 OR H
{
    auto Temp = CPU.A | (CPU.HL >> 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::OR_B5() const // B5 OR L
{
    auto Temp = CPU.A | (CPU.HL & 0xFF);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::OR_B6() const // B6 OR (HL)
{
    auto Temp = CPU.A | CPU.Motherboard.ReadMemory(CPU.HL);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::OR_B7() const // B7 OR A
{
    auto Temp = CPU.A | CPU.A;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_B8() const // B8 CP B
{
    auto Temp = CPU.A - CPU.B;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.B & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_B9() const // B9 CP C
{
    auto Temp = CPU.A - CPU.C;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.C & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_BA() const // BA CP D
{
    auto Temp = CPU.A - CPU.D;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.D & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_BB() const // BB CP E
{
    auto Temp = CPU.A - CPU.E;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.E & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_BC() const // BC CP H
{
    auto Temp = CPU.A - (CPU.HL >> 8);
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - ((CPU.HL >> 8) & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_BD() const // BD CP L
{
    auto Temp = CPU.A - (CPU.HL & 0xFF);
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - ((CPU.HL & 0xFF) & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_BE() const // BE CP (HL)
{
    auto Temp = CPU.A - CPU.Motherboard.ReadMemory(CPU.HL);
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.Motherboard.ReadMemory(CPU.HL) & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::CP_BF() const // BF CP A
{
    auto Temp = CPU.A - CPU.A;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (CPU.A & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::RET_C0() const // C0 RET NZ
{
    if (CPU.GetFlagNZ())
    {
        CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High;
        CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low;
        CPU.SP += 2;
        CPU.SP &= 0xFFFF;
        return 20;
    }
    else
    {
        CPU.PC += 1;
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::POP_C1() const // C1 POP BC
{
    CPU.B = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF); // High;
    CPU.C = CPU.Motherboard.ReadMemory(CPU.SP); // Low;
    CPU.SP += 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 12;
}

uint8 FUnrealBoyOpcodes::JP_C2() const // C2 JP NZ,a16
{
    if (CPU.GetFlagNZ())
    {
        CPU.PC = Value;
        return 16;
    }
    else
    {
        CPU.PC += 3;
        CPU.PC &= 0xFFFF;
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::JP_C3() const // C3 JP a16
{
    CPU.PC = Value;
    return 16;
}

uint8 FUnrealBoyOpcodes::CALL_C4() const // C4 CALL NZ,a16
{
    CPU.PC += 3;
    CPU.PC &= 0xFFFF;
    if (CPU.GetFlagNZ())
    {
        CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
        CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
        CPU.SP -= 2;
        CPU.SP &= 0xFFFF;
        CPU.PC = Value;
        return 24;
    }
    else
    {
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::PUSH_C5() const // C5 PUSH BC
{
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.B); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.C); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 16;
}

uint8 FUnrealBoyOpcodes::ADD_C6() const // C6 ADD A,d8
{
    auto Temp = CPU.A + Value;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (Value & 0xF)) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_C7() const // C7 RST 00H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 0;
    return 16;
}

uint8 FUnrealBoyOpcodes::RET_C8() const // C8 RET Z
{
    if (CPU.GetFlagZ())
    {
        CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High;
        CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low;
        CPU.SP += 2;
        CPU.SP &= 0xFFFF;
        return 20;
    }
    else
    {
        CPU.PC += 1;
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::RET_C9() const // C9 RET
{
    CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High;
    CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low;
    CPU.SP += 2;
    CPU.SP &= 0xFFFF;
    return 16;
}

uint8 FUnrealBoyOpcodes::JP_CA() const // CA JP Z,a16
{
    if (CPU.GetFlagZ())
    {
        CPU.PC = Value;
        return 16;
    }
    else
    {
        CPU.PC += 3;
        CPU.PC &= 0xFFFF;
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::PREFIX_CB() const // CB PREFIX CB
{
    ensureAlwaysMsgf(false, TEXT("CB cannot be called!"));
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CALL_CC() const // CC CALL Z,a16
{
    CPU.PC += 3;
    CPU.PC &= 0xFFFF;
    if (CPU.GetFlagZ())
    {
        CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
        CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
        CPU.SP -= 2;
        CPU.SP &= 0xFFFF;
        CPU.PC = Value;
        return 24;
    }
    else
    {
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::CALL_CD() const // CD CALL a16
{
    CPU.PC += 3;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = Value;
    return 24;
}

uint8 FUnrealBoyOpcodes::ADC_CE() const // CE ADC A,d8
{
    auto Temp = CPU.A + Value + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) + (Value & 0xF) + CPU.GetFlagC()) > 0xF) << FLAG_H;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_CF() const // CF RST 08H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 8;
    return 16;
}

uint8 FUnrealBoyOpcodes::RET_D0() const // D0 RET NC
{
    if (CPU.GetFlagNC())
    {
        CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High;
        CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low;
        CPU.SP += 2;
        CPU.SP &= 0xFFFF;
        return 20;
    }
    else
    {
        CPU.PC += 1;
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::POP_D1() const // D1 POP DE
{
    CPU.D = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF); // High;
    CPU.E = CPU.Motherboard.ReadMemory(CPU.SP); // Low;
    CPU.SP += 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 12;
}

uint8 FUnrealBoyOpcodes::JP_D2() const // D2 JP NC,a16
{
    if (CPU.GetFlagNC())
    {
        CPU.PC = Value;
        return 16;
    }
    else
    {
        CPU.PC += 3;
        CPU.PC &= 0xFFFF;
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::CALL_D4() const // D4 CALL NC,a16
{
    CPU.PC += 3;
    CPU.PC &= 0xFFFF;
    if (CPU.GetFlagNC())
    {
        CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
        CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
        CPU.SP -= 2;
        CPU.SP &= 0xFFFF;
        CPU.PC = Value;
        return 24;
    }
    else
    {
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::PUSH_D5() const // D5 PUSH DE
{
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.D); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.E); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 16;
}

uint8 FUnrealBoyOpcodes::SUB_D6() const // D6 SUB d8
{
    auto Temp = CPU.A - Value;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (Value & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_D7() const // D7 RST 10H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 16;
    return 16;
}

uint8 FUnrealBoyOpcodes::RET_D8() const // D8 RET C
{
    if (CPU.GetFlagC())
    {
        CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High;
        CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low;
        CPU.SP += 2;
        CPU.SP &= 0xFFFF;
        return 20;
    }
    else
    {
        CPU.PC += 1;
        CPU.PC &= 0xFFFF;
        return 8;
    }
}

uint8 FUnrealBoyOpcodes::RETI_D9() const // D9 RETI
{
    CPU.bInterruptMasterEnabled = true;
    CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High;
    CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low;
    CPU.SP += 2;
    CPU.SP &= 0xFFFF;
    return 16;
}

uint8 FUnrealBoyOpcodes::JP_DA() const // DA JP C,a16
{
    if (CPU.GetFlagC())
    {
        CPU.PC = Value;
        return 16;
    }
    else
    {
        CPU.PC += 3;
        CPU.PC &= 0xFFFF;
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::CALL_DC() const // DC CALL C,a16
{
    CPU.PC += 3;
    CPU.PC &= 0xFFFF;
    if (CPU.GetFlagC())
    {
        CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
        CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
        CPU.SP -= 2;
        CPU.SP &= 0xFFFF;
        CPU.PC = Value;
        return 24;
    }
    else
    {
        return 12;
    }
}

uint8 FUnrealBoyOpcodes::SBC_DE() const // DE SBC A,d8
{
    auto Temp = CPU.A - Value - CPU.GetFlagC();
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (Value & 0xF) - CPU.GetFlagC()) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_DF() const // DF RST 18H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 24;
    return 16;
}

uint8 FUnrealBoyOpcodes::LDH_E0() const // E0 LDH (a8),A
{
    CPU.Motherboard.WriteMemory(Value + 0xFF00, CPU.A);
    CPU.PC += 2;
    return 12;
}

uint8 FUnrealBoyOpcodes::POP_E1() const // E1 POP HL
{
    CPU.HL = (CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8) + CPU.Motherboard.ReadMemory(CPU.SP); // High;
    CPU.SP += 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_E2() const // E2 LD (C),A
{
    CPU.Motherboard.WriteMemory(0xFF00 + CPU.C, CPU.A);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::PUSH_E5() const // E5 PUSH HL
{
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.HL >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.HL & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 16;
}

uint8 FUnrealBoyOpcodes::AND_E6() const // E6 AND d8
{
    auto Temp = CPU.A & Value;
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_E7() const // E7 RST 20H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 32;
    return 16;
}

uint8 FUnrealBoyOpcodes::ADD_E8() const // E8 ADD SP,r8
{
    auto Temp = CPU.SP + ((Value ^ 0x80) - 0x80);
    uint8 Flag = 0b00000000;
    Flag += (((CPU.SP & 0xF) + (Value & 0xF)) > 0xF) << FLAG_H;
    Flag += (((CPU.SP & 0xFF) + (Value & 0xFF)) > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFFFF;
    CPU.SP = Temp;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::JP_E9() const // E9 JP (HL)
{
    CPU.PC = CPU.HL;
    return 4;
}

uint8 FUnrealBoyOpcodes::LD_EA() const // EA LD (a16),A
{
    CPU.Motherboard.WriteMemory(Value, CPU.A);
    CPU.PC += 3;
    return 16;
}

uint8 FUnrealBoyOpcodes::XOR_EE() const // EE XOR d8
{
    auto Temp = CPU.A ^ Value;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_EF() const // EF RST 28H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 40;
    return 16;
}

uint8 FUnrealBoyOpcodes::LDH_F0() const // F0 LDH A,(a8)
{
    CPU.A = CPU.Motherboard.ReadMemory(Value + 0xFF00);
    CPU.PC += 2;
    return 12;
}

uint8 FUnrealBoyOpcodes::POP_F1() const // F1 POP AF
{
    CPU.A = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF); // High;
    CPU.F = CPU.Motherboard.ReadMemory(CPU.SP) & 0xF0 & 0xF0; // Low;
    CPU.SP += 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_F2() const // F2 LD A,(C)
{
    CPU.A = CPU.Motherboard.ReadMemory(0xFF00 + CPU.C);
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::DI_F3() const // F3 DI
{
    CPU.bInterruptMasterEnabled = false;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::PUSH_F5() const // F5 PUSH AF
{
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.A); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.F & 0xF0); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC += 1;
    return 16;
}

uint8 FUnrealBoyOpcodes::OR_F6() const // F6 OR d8
{
    auto Temp = CPU.A | Value;
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_F7() const // F7 RST 30H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 48;
    return 16;
}

uint8 FUnrealBoyOpcodes::LD_F8() const // F8 LD HL,SP+r8
{
    CPU.HL = CPU.SP + ((Value ^ 0x80) - 0x80);
    uint16 Temp = CPU.HL;
    uint8 Flag = 0b00000000;
    Flag += (((CPU.SP & 0xF) + (Value & 0xF)) > 0xF) << FLAG_H;
    Flag += (((CPU.SP & 0xFF) + (Value & 0xFF)) > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    CPU.HL &= 0xFFFF;
    CPU.PC += 2;
    return 12;
}

uint8 FUnrealBoyOpcodes::LD_F9() const // F9 LD SP,HL
{
    CPU.SP = CPU.HL;
    CPU.PC += 1;
    return 8;
}

uint8 FUnrealBoyOpcodes::LD_FA() const // FA LD A,(a16)
{
    CPU.A = CPU.Motherboard.ReadMemory(Value);
    CPU.PC += 3;
    return 16;
}

uint8 FUnrealBoyOpcodes::EI_FB() const // FB EI
{
    CPU.bInterruptMasterEnabled = true;
    CPU.PC += 1;
    return 4;
}

uint8 FUnrealBoyOpcodes::CP_FE() const // FE CP d8
{
    auto Temp = CPU.A - Value;
    uint8 Flag = 0b01000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (((CPU.A & 0xF) - (Value & 0xF)) < 0) << FLAG_H;
    Flag += (Temp < 0) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RST_FF() const // FF RST 38H
{
    CPU.PC += 1;
    CPU.PC &= 0xFFFF;
    CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High;
    CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low;
    CPU.SP -= 2;
    CPU.SP &= 0xFFFF;
    CPU.PC = 56;
    return 16;
}

uint8 FUnrealBoyOpcodes::RLC_100() const // 100 RLC B
{
    uint16 Temp = (CPU.B << 1) + (CPU.B >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLC_101() const // 101 RLC C
{
    uint16 Temp = (CPU.C << 1) + (CPU.C >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLC_102() const // 102 RLC D
{
    uint16 Temp = (CPU.D << 1) + (CPU.D >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLC_103() const // 103 RLC E
{
    uint16 Temp = (CPU.E << 1) + (CPU.E >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLC_104() const // 104 RLC H
{
    uint16 Temp = ((CPU.HL >> 8) << 1) + ((CPU.HL >> 8) >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLC_105() const // 105 RLC L
{
    uint16 Temp = ((CPU.HL & 0xFF) << 1) + ((CPU.HL & 0xFF) >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RLC_106() const // 106 RLC (HL)
{
    uint16 Temp = (CPU.Motherboard.ReadMemory(CPU.HL) << 1) + (CPU.Motherboard.ReadMemory(CPU.HL) >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RLC_107() const // 107 RLC A
{
    uint16 Temp = (CPU.A << 1) + (CPU.A >> 7);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRC_108() const // 108 RRC B
{
    uint16 Temp = (CPU.B >> 1) + ((CPU.B & 1) << 7) + ((CPU.B & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRC_109() const // 109 RRC C
{
    uint16 Temp = (CPU.C >> 1) + ((CPU.C & 1) << 7) + ((CPU.C & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRC_10A() const // 10A RRC D
{
    uint16 Temp = (CPU.D >> 1) + ((CPU.D & 1) << 7) + ((CPU.D & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRC_10B() const // 10B RRC E
{
    uint16 Temp = (CPU.E >> 1) + ((CPU.E & 1) << 7) + ((CPU.E & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRC_10C() const // 10C RRC H
{
    uint16 Temp = ((CPU.HL >> 8) >> 1) + (((CPU.HL >> 8) & 1) << 7) + (((CPU.HL >> 8) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRC_10D() const // 10D RRC L
{
    uint16 Temp = ((CPU.HL & 0xFF) >> 1) + (((CPU.HL & 0xFF) & 1) << 7) + (((CPU.HL & 0xFF) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RRC_10E() const // 10E RRC (HL)
{
    uint16 Temp = (CPU.Motherboard.ReadMemory(CPU.HL) >> 1) + ((CPU.Motherboard.ReadMemory(CPU.HL) & 1) << 7) + ((CPU.Motherboard.ReadMemory(CPU.HL) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RRC_10F() const // 10F RRC A
{
    uint16 Temp = (CPU.A >> 1) + ((CPU.A & 1) << 7) + ((CPU.A & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RL_110() const // 110 RL B
{
    uint16 Temp = (CPU.B << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RL_111() const // 111 RL C
{
    uint16 Temp = (CPU.C << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RL_112() const // 112 RL D
{
    uint16 Temp = (CPU.D << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RL_113() const // 113 RL E
{
    uint16 Temp = (CPU.E << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RL_114() const // 114 RL H
{
    uint16 Temp = ((CPU.HL >> 8) << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RL_115() const // 115 RL L
{
    uint16 Temp = ((CPU.HL & 0xFF) << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RL_116() const // 116 RL (HL)
{
    uint16 Temp = (CPU.Motherboard.ReadMemory(CPU.HL) << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RL_117() const // 117 RL A
{
    uint16 Temp = (CPU.A << 1) + CPU.GetFlagC();
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RR_118() const // 118 RR B
{
    uint16 Temp = (CPU.B >> 1) + (CPU.GetFlagC() << 7) + ((CPU.B & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RR_119() const // 119 RR C
{
    uint16 Temp = (CPU.C >> 1) + (CPU.GetFlagC() << 7) + ((CPU.C & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RR_11A() const // 11A RR D
{
    uint16 Temp = (CPU.D >> 1) + (CPU.GetFlagC() << 7) + ((CPU.D & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RR_11B() const // 11B RR E
{
    uint16 Temp = (CPU.E >> 1) + (CPU.GetFlagC() << 7) + ((CPU.E & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RR_11C() const // 11C RR H
{
    uint16 Temp = ((CPU.HL >> 8) >> 1) + (CPU.GetFlagC() << 7) + (((CPU.HL >> 8) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RR_11D() const // 11D RR L
{
    uint16 Temp = ((CPU.HL & 0xFF) >> 1) + (CPU.GetFlagC() << 7) + (((CPU.HL & 0xFF) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RR_11E() const // 11E RR (HL)
{
    uint16 Temp = (CPU.Motherboard.ReadMemory(CPU.HL) >> 1) + (CPU.GetFlagC() << 7) + ((CPU.Motherboard.ReadMemory(CPU.HL) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RR_11F() const // 11F RR A
{
    uint16 Temp = (CPU.A >> 1) + (CPU.GetFlagC() << 7) + ((CPU.A & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SLA_120() const // 120 SLA B
{
    uint16 Temp = (CPU.B << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SLA_121() const // 121 SLA C
{
    uint16 Temp = (CPU.C << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SLA_122() const // 122 SLA D
{
    uint16 Temp = (CPU.D << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SLA_123() const // 123 SLA E
{
    uint16 Temp = (CPU.E << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SLA_124() const // 124 SLA H
{
    uint16 Temp = ((CPU.HL >> 8) << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SLA_125() const // 125 SLA L
{
    uint16 Temp = ((CPU.HL & 0xFF) << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SLA_126() const // 126 SLA (HL)
{
    uint16 Temp = (CPU.Motherboard.ReadMemory(CPU.HL) << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SLA_127() const // 127 SLA A
{
    uint16 Temp = (CPU.A << 1);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRA_128() const // 128 SRA B
{
    uint16 Temp = ((CPU.B >> 1) | (CPU.B & 0x80)) + ((CPU.B & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRA_129() const // 129 SRA C
{
    uint16 Temp = ((CPU.C >> 1) | (CPU.C & 0x80)) + ((CPU.C & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRA_12A() const // 12A SRA D
{
    uint16 Temp = ((CPU.D >> 1) | (CPU.D & 0x80)) + ((CPU.D & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRA_12B() const // 12B SRA E
{
    uint16 Temp = ((CPU.E >> 1) | (CPU.E & 0x80)) + ((CPU.E & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRA_12C() const // 12C SRA H
{
    uint16 Temp = (((CPU.HL >> 8) >> 1) | ((CPU.HL >> 8) & 0x80)) + (((CPU.HL >> 8) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRA_12D() const // 12D SRA L
{
    uint16 Temp = (((CPU.HL & 0xFF) >> 1) | ((CPU.HL & 0xFF) & 0x80)) + (((CPU.HL & 0xFF) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRA_12E() const // 12E SRA (HL)
{
    uint16 Temp = ((CPU.Motherboard.ReadMemory(CPU.HL) >> 1) | (CPU.Motherboard.ReadMemory(CPU.HL) & 0x80)) + ((CPU.Motherboard.ReadMemory(CPU.HL) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SRA_12F() const // 12F SRA A
{
    uint16 Temp = ((CPU.A >> 1) | (CPU.A & 0x80)) + ((CPU.A & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SWAP_130() const // 130 SWAP B
{
    uint16 Temp = ((CPU.B & 0xF0) >> 4) | ((CPU.B & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SWAP_131() const // 131 SWAP C
{
    uint16 Temp = ((CPU.C & 0xF0) >> 4) | ((CPU.C & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SWAP_132() const // 132 SWAP D
{
    uint16 Temp = ((CPU.D & 0xF0) >> 4) | ((CPU.D & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SWAP_133() const // 133 SWAP E
{
    uint16 Temp = ((CPU.E & 0xF0) >> 4) | ((CPU.E & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SWAP_134() const // 134 SWAP H
{
    uint16 Temp = (((CPU.HL >> 8) & 0xF0) >> 4) | (((CPU.HL >> 8) & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SWAP_135() const // 135 SWAP L
{
    uint16 Temp = (((CPU.HL & 0xFF) & 0xF0) >> 4) | (((CPU.HL & 0xFF) & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SWAP_136() const // 136 SWAP (HL)
{
    uint16 Temp = ((CPU.Motherboard.ReadMemory(CPU.HL) & 0xF0) >> 4) | ((CPU.Motherboard.ReadMemory(CPU.HL) & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SWAP_137() const // 137 SWAP A
{
    uint16 Temp = ((CPU.A & 0xF0) >> 4) | ((CPU.A & 0x0F) << 4);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRL_138() const // 138 SRL B
{
    uint16 Temp = (CPU.B >> 1) + ((CPU.B & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRL_139() const // 139 SRL C
{
    uint16 Temp = (CPU.C >> 1) + ((CPU.C & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRL_13A() const // 13A SRL D
{
    uint16 Temp = (CPU.D >> 1) + ((CPU.D & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRL_13B() const // 13B SRL E
{
    uint16 Temp = (CPU.E >> 1) + ((CPU.E & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRL_13C() const // 13C SRL H
{
    uint16 Temp = ((CPU.HL >> 8) >> 1) + (((CPU.HL >> 8) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRL_13D() const // 13D SRL L
{
    uint16 Temp = ((CPU.HL & 0xFF) >> 1) + (((CPU.HL & 0xFF) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SRL_13E() const // 13E SRL (HL)
{
    uint16 Temp = (CPU.Motherboard.ReadMemory(CPU.HL) >> 1) + ((CPU.Motherboard.ReadMemory(CPU.HL) & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SRL_13F() const // 13F SRL A
{
    uint16 Temp = (CPU.A >> 1) + ((CPU.A & 1) << 8);
    uint8 Flag = 0b00000000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    Flag += (Temp > 0xFF) << FLAG_C;
    CPU.F &= 0b00000000;
    CPU.F |= Flag;
    Temp &= 0xFF;
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_140() const // 140 BIT 0,B
{
    uint16 Temp = CPU.B & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_141() const // 141 BIT 0,C
{
    uint16 Temp = CPU.C & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_142() const // 142 BIT 0,D
{
    uint16 Temp = CPU.D & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_143() const // 143 BIT 0,E
{
    uint16 Temp = CPU.E & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_144() const // 144 BIT 0,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_145() const // 145 BIT 0,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_146() const // 146 BIT 0,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_147() const // 147 BIT 0,A
{
    uint16 Temp = CPU.A & (1 << 0);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_148() const // 148 BIT 1,B
{
    uint16 Temp = CPU.B & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_149() const // 149 BIT 1,C
{
    uint16 Temp = CPU.C & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_14A() const // 14A BIT 1,D
{
    uint16 Temp = CPU.D & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_14B() const // 14B BIT 1,E
{
    uint16 Temp = CPU.E & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_14C() const // 14C BIT 1,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_14D() const // 14D BIT 1,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_14E() const // 14E BIT 1,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_14F() const // 14F BIT 1,A
{
    uint16 Temp = CPU.A & (1 << 1);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_150() const // 150 BIT 2,B
{
    uint16 Temp = CPU.B & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_151() const // 151 BIT 2,C
{
    uint16 Temp = CPU.C & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_152() const // 152 BIT 2,D
{
    uint16 Temp = CPU.D & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_153() const // 153 BIT 2,E
{
    uint16 Temp = CPU.E & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_154() const // 154 BIT 2,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_155() const // 155 BIT 2,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_156() const // 156 BIT 2,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_157() const // 157 BIT 2,A
{
    uint16 Temp = CPU.A & (1 << 2);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_158() const // 158 BIT 3,B
{
    uint16 Temp = CPU.B & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_159() const // 159 BIT 3,C
{
    uint16 Temp = CPU.C & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_15A() const // 15A BIT 3,D
{
    uint16 Temp = CPU.D & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_15B() const // 15B BIT 3,E
{
    uint16 Temp = CPU.E & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_15C() const // 15C BIT 3,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_15D() const // 15D BIT 3,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_15E() const // 15E BIT 3,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_15F() const // 15F BIT 3,A
{
    uint16 Temp = CPU.A & (1 << 3);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_160() const // 160 BIT 4,B
{
    uint16 Temp = CPU.B & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_161() const // 161 BIT 4,C
{
    uint16 Temp = CPU.C & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_162() const // 162 BIT 4,D
{
    uint16 Temp = CPU.D & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_163() const // 163 BIT 4,E
{
    uint16 Temp = CPU.E & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_164() const // 164 BIT 4,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_165() const // 165 BIT 4,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_166() const // 166 BIT 4,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_167() const // 167 BIT 4,A
{
    uint16 Temp = CPU.A & (1 << 4);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_168() const // 168 BIT 5,B
{
    uint16 Temp = CPU.B & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_169() const // 169 BIT 5,C
{
    uint16 Temp = CPU.C & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_16A() const // 16A BIT 5,D
{
    uint16 Temp = CPU.D & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_16B() const // 16B BIT 5,E
{
    uint16 Temp = CPU.E & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_16C() const // 16C BIT 5,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_16D() const // 16D BIT 5,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_16E() const // 16E BIT 5,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_16F() const // 16F BIT 5,A
{
    uint16 Temp = CPU.A & (1 << 5);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_170() const // 170 BIT 6,B
{
    uint16 Temp = CPU.B & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_171() const // 171 BIT 6,C
{
    uint16 Temp = CPU.C & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_172() const // 172 BIT 6,D
{
    uint16 Temp = CPU.D & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_173() const // 173 BIT 6,E
{
    uint16 Temp = CPU.E & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_174() const // 174 BIT 6,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_175() const // 175 BIT 6,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_176() const // 176 BIT 6,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_177() const // 177 BIT 6,A
{
    uint16 Temp = CPU.A & (1 << 6);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_178() const // 178 BIT 7,B
{
    uint16 Temp = CPU.B & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_179() const // 179 BIT 7,C
{
    uint16 Temp = CPU.C & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_17A() const // 17A BIT 7,D
{
    uint16 Temp = CPU.D & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_17B() const // 17B BIT 7,E
{
    uint16 Temp = CPU.E & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_17C() const // 17C BIT 7,H
{
    uint16 Temp = (CPU.HL >> 8) & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_17D() const // 17D BIT 7,L
{
    uint16 Temp = (CPU.HL & 0xFF) & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::BIT_17E() const // 17E BIT 7,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::BIT_17F() const // 17F BIT 7,A
{
    uint16 Temp = CPU.A & (1 << 7);
    uint8 Flag = 0b00100000;
    Flag += ((Temp & 0xFF) == 0) << FLAG_Z;
    CPU.F &= 0b00010000;
    CPU.F |= Flag;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_180() const // 180 RES 0,B
{
    uint16 Temp = CPU.B & ~(1 << 0);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_181() const // 181 RES 0,C
{
    uint16 Temp = CPU.C & ~(1 << 0);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_182() const // 182 RES 0,D
{
    uint16 Temp = CPU.D & ~(1 << 0);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_183() const // 183 RES 0,E
{
    uint16 Temp = CPU.E & ~(1 << 0);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_184() const // 184 RES 0,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 0);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_185() const // 185 RES 0,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 0);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_186() const // 186 RES 0,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 0);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_187() const // 187 RES 0,A
{
    uint16 Temp = CPU.A & ~(1 << 0);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_188() const // 188 RES 1,B
{
    uint16 Temp = CPU.B & ~(1 << 1);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_189() const // 189 RES 1,C
{
    uint16 Temp = CPU.C & ~(1 << 1);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_18A() const // 18A RES 1,D
{
    uint16 Temp = CPU.D & ~(1 << 1);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_18B() const // 18B RES 1,E
{
    uint16 Temp = CPU.E & ~(1 << 1);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_18C() const // 18C RES 1,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 1);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_18D() const // 18D RES 1,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 1);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_18E() const // 18E RES 1,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 1);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_18F() const // 18F RES 1,A
{
    uint16 Temp = CPU.A & ~(1 << 1);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_190() const // 190 RES 2,B
{
    uint16 Temp = CPU.B & ~(1 << 2);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_191() const // 191 RES 2,C
{
    uint16 Temp = CPU.C & ~(1 << 2);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_192() const // 192 RES 2,D
{
    uint16 Temp = CPU.D & ~(1 << 2);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_193() const // 193 RES 2,E
{
    uint16 Temp = CPU.E & ~(1 << 2);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_194() const // 194 RES 2,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 2);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_195() const // 195 RES 2,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 2);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_196() const // 196 RES 2,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 2);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_197() const // 197 RES 2,A
{
    uint16 Temp = CPU.A & ~(1 << 2);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_198() const // 198 RES 3,B
{
    uint16 Temp = CPU.B & ~(1 << 3);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_199() const // 199 RES 3,C
{
    uint16 Temp = CPU.C & ~(1 << 3);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_19A() const // 19A RES 3,D
{
    uint16 Temp = CPU.D & ~(1 << 3);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_19B() const // 19B RES 3,E
{
    uint16 Temp = CPU.E & ~(1 << 3);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_19C() const // 19C RES 3,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 3);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_19D() const // 19D RES 3,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 3);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_19E() const // 19E RES 3,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 3);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_19F() const // 19F RES 3,A
{
    uint16 Temp = CPU.A & ~(1 << 3);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A0() const // 1A0 RES 4,B
{
    uint16 Temp = CPU.B & ~(1 << 4);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A1() const // 1A1 RES 4,C
{
    uint16 Temp = CPU.C & ~(1 << 4);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A2() const // 1A2 RES 4,D
{
    uint16 Temp = CPU.D & ~(1 << 4);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A3() const // 1A3 RES 4,E
{
    uint16 Temp = CPU.E & ~(1 << 4);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A4() const // 1A4 RES 4,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 4);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A5() const // 1A5 RES 4,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 4);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A6() const // 1A6 RES 4,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 4);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_1A7() const // 1A7 RES 4,A
{
    uint16 Temp = CPU.A & ~(1 << 4);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A8() const // 1A8 RES 5,B
{
    uint16 Temp = CPU.B & ~(1 << 5);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1A9() const // 1A9 RES 5,C
{
    uint16 Temp = CPU.C & ~(1 << 5);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1AA() const // 1AA RES 5,D
{
    uint16 Temp = CPU.D & ~(1 << 5);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1AB() const // 1AB RES 5,E
{
    uint16 Temp = CPU.E & ~(1 << 5);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1AC() const // 1AC RES 5,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 5);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1AD() const // 1AD RES 5,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 5);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1AE() const // 1AE RES 5,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 5);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_1AF() const // 1AF RES 5,A
{
    uint16 Temp = CPU.A & ~(1 << 5);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B0() const // 1B0 RES 6,B
{
    uint16 Temp = CPU.B & ~(1 << 6);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B1() const // 1B1 RES 6,C
{
    uint16 Temp = CPU.C & ~(1 << 6);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B2() const // 1B2 RES 6,D
{
    uint16 Temp = CPU.D & ~(1 << 6);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B3() const // 1B3 RES 6,E
{
    uint16 Temp = CPU.E & ~(1 << 6);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B4() const // 1B4 RES 6,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 6);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B5() const // 1B5 RES 6,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 6);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B6() const // 1B6 RES 6,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 6);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_1B7() const // 1B7 RES 6,A
{
    uint16 Temp = CPU.A & ~(1 << 6);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B8() const // 1B8 RES 7,B
{
    uint16 Temp = CPU.B & ~(1 << 7);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1B9() const // 1B9 RES 7,C
{
    uint16 Temp = CPU.C & ~(1 << 7);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1BA() const // 1BA RES 7,D
{
    uint16 Temp = CPU.D & ~(1 << 7);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1BB() const // 1BB RES 7,E
{
    uint16 Temp = CPU.E & ~(1 << 7);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1BC() const // 1BC RES 7,H
{
    uint16 Temp = (CPU.HL >> 8) & ~(1 << 7);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1BD() const // 1BD RES 7,L
{
    uint16 Temp = (CPU.HL & 0xFF) & ~(1 << 7);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::RES_1BE() const // 1BE RES 7,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) & ~(1 << 7);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::RES_1BF() const // 1BF RES 7,A
{
    uint16 Temp = CPU.A & ~(1 << 7);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C0() const // 1C0 SET 0,B
{
    uint16 Temp = CPU.B | (1 << 0);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C1() const // 1C1 SET 0,C
{
    uint16 Temp = CPU.C | (1 << 0);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C2() const // 1C2 SET 0,D
{
    uint16 Temp = CPU.D | (1 << 0);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C3() const // 1C3 SET 0,E
{
    uint16 Temp = CPU.E | (1 << 0);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C4() const // 1C4 SET 0,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 0);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C5() const // 1C5 SET 0,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 0);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C6() const // 1C6 SET 0,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 0);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1C7() const // 1C7 SET 0,A
{
    uint16 Temp = CPU.A | (1 << 0);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C8() const // 1C8 SET 1,B
{
    uint16 Temp = CPU.B | (1 << 1);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1C9() const // 1C9 SET 1,C
{
    uint16 Temp = CPU.C | (1 << 1);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1CA() const // 1CA SET 1,D
{
    uint16 Temp = CPU.D | (1 << 1);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1CB() const // 1CB SET 1,E
{
    uint16 Temp = CPU.E | (1 << 1);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1CC() const // 1CC SET 1,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 1);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1CD() const // 1CD SET 1,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 1);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1CE() const // 1CE SET 1,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 1);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1CF() const // 1CF SET 1,A
{
    uint16 Temp = CPU.A | (1 << 1);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D0() const // 1D0 SET 2,B
{
    uint16 Temp = CPU.B | (1 << 2);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D1() const // 1D1 SET 2,C
{
    uint16 Temp = CPU.C | (1 << 2);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D2() const // 1D2 SET 2,D
{
    uint16 Temp = CPU.D | (1 << 2);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D3() const // 1D3 SET 2,E
{
    uint16 Temp = CPU.E | (1 << 2);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D4() const // 1D4 SET 2,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 2);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D5() const // 1D5 SET 2,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 2);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D6() const // 1D6 SET 2,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 2);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1D7() const // 1D7 SET 2,A
{
    uint16 Temp = CPU.A | (1 << 2);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D8() const // 1D8 SET 3,B
{
    uint16 Temp = CPU.B | (1 << 3);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1D9() const // 1D9 SET 3,C
{
    uint16 Temp = CPU.C | (1 << 3);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1DA() const // 1DA SET 3,D
{
    uint16 Temp = CPU.D | (1 << 3);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1DB() const // 1DB SET 3,E
{
    uint16 Temp = CPU.E | (1 << 3);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1DC() const // 1DC SET 3,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 3);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1DD() const // 1DD SET 3,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 3);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1DE() const // 1DE SET 3,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 3);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1DF() const // 1DF SET 3,A
{
    uint16 Temp = CPU.A | (1 << 3);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E0() const // 1E0 SET 4,B
{
    uint16 Temp = CPU.B | (1 << 4);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E1() const // 1E1 SET 4,C
{
    uint16 Temp = CPU.C | (1 << 4);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E2() const // 1E2 SET 4,D
{
    uint16 Temp = CPU.D | (1 << 4);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E3() const // 1E3 SET 4,E
{
    uint16 Temp = CPU.E | (1 << 4);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E4() const // 1E4 SET 4,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 4);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E5() const // 1E5 SET 4,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 4);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E6() const // 1E6 SET 4,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 4);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1E7() const // 1E7 SET 4,A
{
    uint16 Temp = CPU.A | (1 << 4);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E8() const // 1E8 SET 5,B
{
    uint16 Temp = CPU.B | (1 << 5);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1E9() const // 1E9 SET 5,C
{
    uint16 Temp = CPU.C | (1 << 5);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1EA() const // 1EA SET 5,D
{
    uint16 Temp = CPU.D | (1 << 5);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1EB() const // 1EB SET 5,E
{
    uint16 Temp = CPU.E | (1 << 5);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1EC() const // 1EC SET 5,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 5);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1ED() const // 1ED SET 5,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 5);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1EE() const // 1EE SET 5,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 5);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1EF() const // 1EF SET 5,A
{
    uint16 Temp = CPU.A | (1 << 5);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F0() const // 1F0 SET 6,B
{
    uint16 Temp = CPU.B | (1 << 6);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F1() const // 1F1 SET 6,C
{
    uint16 Temp = CPU.C | (1 << 6);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F2() const // 1F2 SET 6,D
{
    uint16 Temp = CPU.D | (1 << 6);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F3() const // 1F3 SET 6,E
{
    uint16 Temp = CPU.E | (1 << 6);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F4() const // 1F4 SET 6,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 6);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F5() const // 1F5 SET 6,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 6);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F6() const // 1F6 SET 6,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 6);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1F7() const // 1F7 SET 6,A
{
    uint16 Temp = CPU.A | (1 << 6);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F8() const // 1F8 SET 7,B
{
    uint16 Temp = CPU.B | (1 << 7);
    CPU.B = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1F9() const // 1F9 SET 7,C
{
    uint16 Temp = CPU.C | (1 << 7);
    CPU.C = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1FA() const // 1FA SET 7,D
{
    uint16 Temp = CPU.D | (1 << 7);
    CPU.D = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1FB() const // 1FB SET 7,E
{
    uint16 Temp = CPU.E | (1 << 7);
    CPU.E = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1FC() const // 1FC SET 7,H
{
    uint16 Temp = (CPU.HL >> 8) | (1 << 7);
    CPU.HL = (CPU.HL & 0x00FF) | (Temp << 8);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1FD() const // 1FD SET 7,L
{
    uint16 Temp = (CPU.HL & 0xFF) | (1 << 7);
    CPU.HL = (CPU.HL & 0xFF00) | (Temp & 0xFF);
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::SET_1FE() const // 1FE SET 7,(HL)
{
    uint16 Temp = CPU.Motherboard.ReadMemory(CPU.HL) | (1 << 7);
    CPU.Motherboard.WriteMemory(CPU.HL, Temp);
    CPU.PC += 2;
    return 16;
}

uint8 FUnrealBoyOpcodes::SET_1FF() const // 1FF SET 7,A
{
    uint16 Temp = CPU.A | (1 << 7);
    CPU.A = Temp;
    CPU.PC += 2;
    return 8;
}

uint8 FUnrealBoyOpcodes::NoOpcode()
{
    return 0;
}

void FUnrealBoyOpcodes::InitOpcodeTable()
{
    OpcodeTable = TArray<FOpcodeFunction> {
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NOP_00),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_01),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_02),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_03),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_04),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_05),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_06),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLCA_07),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_08),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_09),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_0A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_0B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_0C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_0D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_0E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRCA_0F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::STOP_10),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_11),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_12),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_13),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_14),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_15),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_16),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLA_17),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JR_18),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_19),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_1A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_1B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_1C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_1D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_1E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRA_1F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JR_20),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_21),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_22),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_23),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_24),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_25),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_26),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DAA_27),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JR_28),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_29),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_2A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_2B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_2C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_2D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_2E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CPL_2F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JR_30),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_31),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_32),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_33),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_34),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_35),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_36),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SCF_37),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JR_38),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_39),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_3A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_3B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::INC_3C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DEC_3D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_3E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CCF_3F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_40),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_41),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_42),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_43),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_44),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_45),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_46),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_47),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_48),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_49),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_4A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_4B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_4C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_4D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_4E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_4F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_50),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_51),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_52),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_53),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_54),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_55),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_56),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_57),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_58),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_59),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_5A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_5B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_5C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_5D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_5E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_5F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_60),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_61),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_62),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_63),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_64),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_65),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_66),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_67),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_68),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_69),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_6A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_6B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_6C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_6D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_6E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_6F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_70),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_71),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_72),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_73),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_74),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_75),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::HALT_76),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_77),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_78),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_79),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_7A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_7B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_7C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_7D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_7E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_7F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_80),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_81),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_82),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_83),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_84),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_85),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_86),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_87),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_88),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_89),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_8A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_8B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_8C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_8D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_8E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_8F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_90),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_91),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_92),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_93),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_94),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_95),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_96),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_97),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_98),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_99),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_9A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_9B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_9C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_9D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_9E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_9F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_A7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_A8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_A9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_AA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_AB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_AC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_AD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_AE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_AF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_B7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_B8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_B9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_BA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_BB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_BC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_BD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_BE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_BF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RET_C0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::POP_C1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JP_C2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JP_C3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CALL_C4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::PUSH_C5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_C6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_C7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RET_C8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RET_C9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JP_CA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::PREFIX_CB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CALL_CC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CALL_CD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADC_CE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_CF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RET_D0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::POP_D1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JP_D2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CALL_D4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::PUSH_D5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SUB_D6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_D7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RET_D8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RETI_D9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JP_DA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CALL_DC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SBC_DE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_DF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LDH_E0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::POP_E1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_E2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::PUSH_E5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::AND_E6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_E7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::ADD_E8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::JP_E9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_EA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::XOR_EE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_EF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LDH_F0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::POP_F1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_F2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::DI_F3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::PUSH_F5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::OR_F6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_F7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_F8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_F9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::LD_FA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::EI_FB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::NoOpcode),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::CP_FE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RST_FF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_100),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_101),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_102),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_103),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_104),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_105),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_106),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RLC_107),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_108),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_109),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_10A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_10B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_10C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_10D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_10E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RRC_10F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_110),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_111),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_112),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_113),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_114),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_115),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_116),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RL_117),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_118),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_119),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_11A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_11B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_11C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_11D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_11E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RR_11F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_120),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_121),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_122),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_123),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_124),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_125),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_126),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SLA_127),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_128),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_129),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_12A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_12B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_12C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_12D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_12E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRA_12F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_130),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_131),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_132),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_133),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_134),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_135),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_136),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SWAP_137),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_138),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_139),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_13A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_13B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_13C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_13D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_13E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SRL_13F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_140),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_141),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_142),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_143),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_144),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_145),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_146),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_147),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_148),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_149),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_14A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_14B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_14C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_14D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_14E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_14F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_150),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_151),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_152),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_153),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_154),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_155),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_156),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_157),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_158),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_159),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_15A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_15B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_15C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_15D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_15E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_15F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_160),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_161),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_162),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_163),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_164),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_165),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_166),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_167),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_168),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_169),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_16A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_16B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_16C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_16D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_16E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_16F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_170),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_171),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_172),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_173),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_174),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_175),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_176),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_177),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_178),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_179),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_17A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_17B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_17C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_17D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_17E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::BIT_17F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_180),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_181),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_182),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_183),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_184),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_185),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_186),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_187),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_188),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_189),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_18A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_18B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_18C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_18D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_18E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_18F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_190),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_191),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_192),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_193),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_194),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_195),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_196),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_197),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_198),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_199),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_19A),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_19B),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_19C),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_19D),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_19E),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_19F),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1A9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1AA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1AB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1AC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1AD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1AE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1AF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1B9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1BA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1BB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1BC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1BD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1BE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::RES_1BF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1C9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1CA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1CB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1CC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1CD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1CE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1CF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1D9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1DA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1DB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1DC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1DD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1DE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1DF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1E9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1EA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1EB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1EC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1ED),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1EE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1EF),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F0),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F1),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F2),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F3),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F4),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F5),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F6),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F7),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F8),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1F9),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1FA),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1FB),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1FC),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1FD),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1FE),
        FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::SET_1FF),
        };
}
uint8 FUnrealBoyOpcodes::GetOpcodeLength(uint16 Opcode)
{
    static TArray<uint8> OpcodeLengths = TArray<uint8> {
        1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1,
        2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
        2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
        2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
        1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0, 2, 1,
        2, 1, 1, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1,
        2, 1, 1, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        };
    return OpcodeLengths[Opcode];
}

uint8 FUnrealBoyOpcodes::ExecuteOpcode(uint16 Opcode)
{
	Value = 0;
	const uint16 PC = CPU.PC;

	const uint8 OpcodeLength = GetOpcodeLength(Opcode);
	if (OpcodeLength == 2)
	{
		// 8-bit immediate
		Value = CPU.Motherboard.ReadMemory(PC + 1);
	}
	else if (OpcodeLength == 3)
	{
		// 16-bit immediate
		// Flips order of values due to big-endian
		const uint16 A = CPU.Motherboard.ReadMemory(PC + 2);
		const uint8 B = CPU.Motherboard.ReadMemory(PC + 1);
		Value = (A << 8) + B;
	}
	else
	{
		check(OpcodeLength < 3);
	}

	check(OpcodeTable.IsValidIndex(Opcode));
	const FOpcodeFunction& OpFunction = OpcodeTable[Opcode];
	return OpFunction.Execute();
}

FString FUnrealBoyOpcodes::GetCommandName(uint16 Opcode)
{
    static TArray<FString> CpuCommands = TArray<FString> {
        "NOP",
        "LD BC,d16",
        "LD (BC),A",
        "INC BC",
        "INC B",
        "DEC B",
        "LD B,d8",
        "RLCA",
        "LD (a16),SP",
        "ADD HL,BC",
        "LD A,(BC)",
        "DEC BC",
        "INC C",
        "DEC C",
        "LD C,d8",
        "RRCA",
        "STOP 0",
        "LD DE,d16",
        "LD (DE),A",
        "INC DE",
        "INC D",
        "DEC D",
        "LD D,d8",
        "RLA",
        "JR r8",
        "ADD HL,DE",
        "LD A,(DE)",
        "DEC DE",
        "INC E",
        "DEC E",
        "LD E,d8",
        "RRA",
        "JR NZ,r8",
        "LD HL,d16",
        "LD (HL+),A",
        "INC HL",
        "INC H",
        "DEC H",
        "LD H,d8",
        "DAA",
        "JR Z,r8",
        "ADD HL,HL",
        "LD A,(HL+)",
        "DEC HL",
        "INC L",
        "DEC L",
        "LD L,d8",
        "CPL",
        "JR NC,r8",
        "LD SP,d16",
        "LD (HL-),A",
        "INC SP",
        "INC (HL)",
        "DEC (HL)",
        "LD (HL),d8",
        "SCF",
        "JR C,r8",
        "ADD HL,SP",
        "LD A,(HL-)",
        "DEC SP",
        "INC A",
        "DEC A",
        "LD A,d8",
        "CCF",
        "LD B,B",
        "LD B,C",
        "LD B,D",
        "LD B,E",
        "LD B,H",
        "LD B,L",
        "LD B,(HL)",
        "LD B,A",
        "LD C,B",
        "LD C,C",
        "LD C,D",
        "LD C,E",
        "LD C,H",
        "LD C,L",
        "LD C,(HL)",
        "LD C,A",
        "LD D,B",
        "LD D,C",
        "LD D,D",
        "LD D,E",
        "LD D,H",
        "LD D,L",
        "LD D,(HL)",
        "LD D,A",
        "LD E,B",
        "LD E,C",
        "LD E,D",
        "LD E,E",
        "LD E,H",
        "LD E,L",
        "LD E,(HL)",
        "LD E,A",
        "LD H,B",
        "LD H,C",
        "LD H,D",
        "LD H,E",
        "LD H,H",
        "LD H,L",
        "LD H,(HL)",
        "LD H,A",
        "LD L,B",
        "LD L,C",
        "LD L,D",
        "LD L,E",
        "LD L,H",
        "LD L,L",
        "LD L,(HL)",
        "LD L,A",
        "LD (HL),B",
        "LD (HL),C",
        "LD (HL),D",
        "LD (HL),E",
        "LD (HL),H",
        "LD (HL),L",
        "HALT",
        "LD (HL),A",
        "LD A,B",
        "LD A,C",
        "LD A,D",
        "LD A,E",
        "LD A,H",
        "LD A,L",
        "LD A,(HL)",
        "LD A,A",
        "ADD A,B",
        "ADD A,C",
        "ADD A,D",
        "ADD A,E",
        "ADD A,H",
        "ADD A,L",
        "ADD A,(HL)",
        "ADD A,A",
        "ADC A,B",
        "ADC A,C",
        "ADC A,D",
        "ADC A,E",
        "ADC A,H",
        "ADC A,L",
        "ADC A,(HL)",
        "ADC A,A",
        "SUB B",
        "SUB C",
        "SUB D",
        "SUB E",
        "SUB H",
        "SUB L",
        "SUB (HL)",
        "SUB A",
        "SBC A,B",
        "SBC A,C",
        "SBC A,D",
        "SBC A,E",
        "SBC A,H",
        "SBC A,L",
        "SBC A,(HL)",
        "SBC A,A",
        "AND B",
        "AND C",
        "AND D",
        "AND E",
        "AND H",
        "AND L",
        "AND (HL)",
        "AND A",
        "XOR B",
        "XOR C",
        "XOR D",
        "XOR E",
        "XOR H",
        "XOR L",
        "XOR (HL)",
        "XOR A",
        "OR B",
        "OR C",
        "OR D",
        "OR E",
        "OR H",
        "OR L",
        "OR (HL)",
        "OR A",
        "CP B",
        "CP C",
        "CP D",
        "CP E",
        "CP H",
        "CP L",
        "CP (HL)",
        "CP A",
        "RET NZ",
        "POP BC",
        "JP NZ,a16",
        "JP a16",
        "CALL NZ,a16",
        "PUSH BC",
        "ADD A,d8",
        "RST 00H",
        "RET Z",
        "RET",
        "JP Z,a16",
        "PREFIX CB",
        "CALL Z,a16",
        "CALL a16",
        "ADC A,d8",
        "RST 08H",
        "RET NC",
        "POP DE",
        "JP NC,a16",
        "",
        "CALL NC,a16",
        "PUSH DE",
        "SUB d8",
        "RST 10H",
        "RET C",
        "RETI",
        "JP C,a16",
        "",
        "CALL C,a16",
        "",
        "SBC A,d8",
        "RST 18H",
        "LDH (a8),A",
        "POP HL",
        "LD (C),A",
        "",
        "",
        "PUSH HL",
        "AND d8",
        "RST 20H",
        "ADD SP,r8",
        "JP (HL)",
        "LD (a16),A",
        "",
        "",
        "",
        "XOR d8",
        "RST 28H",
        "LDH A,(a8)",
        "POP AF",
        "LD A,(C)",
        "DI",
        "",
        "PUSH AF",
        "OR d8",
        "RST 30H",
        "LD HL,SP+r8",
        "LD SP,HL",
        "LD A,(a16)",
        "EI",
        "",
        "",
        "CP d8",
        "RST 38H",
        "RLC B",
        "RLC C",
        "RLC D",
        "RLC E",
        "RLC H",
        "RLC L",
        "RLC (HL)",
        "RLC A",
        "RRC B",
        "RRC C",
        "RRC D",
        "RRC E",
        "RRC H",
        "RRC L",
        "RRC (HL)",
        "RRC A",
        "RL B",
        "RL C",
        "RL D",
        "RL E",
        "RL H",
        "RL L",
        "RL (HL)",
        "RL A",
        "RR B",
        "RR C",
        "RR D",
        "RR E",
        "RR H",
        "RR L",
        "RR (HL)",
        "RR A",
        "SLA B",
        "SLA C",
        "SLA D",
        "SLA E",
        "SLA H",
        "SLA L",
        "SLA (HL)",
        "SLA A",
        "SRA B",
        "SRA C",
        "SRA D",
        "SRA E",
        "SRA H",
        "SRA L",
        "SRA (HL)",
        "SRA A",
        "SWAP B",
        "SWAP C",
        "SWAP D",
        "SWAP E",
        "SWAP H",
        "SWAP L",
        "SWAP (HL)",
        "SWAP A",
        "SRL B",
        "SRL C",
        "SRL D",
        "SRL E",
        "SRL H",
        "SRL L",
        "SRL (HL)",
        "SRL A",
        "BIT 0,B",
        "BIT 0,C",
        "BIT 0,D",
        "BIT 0,E",
        "BIT 0,H",
        "BIT 0,L",
        "BIT 0,(HL)",
        "BIT 0,A",
        "BIT 1,B",
        "BIT 1,C",
        "BIT 1,D",
        "BIT 1,E",
        "BIT 1,H",
        "BIT 1,L",
        "BIT 1,(HL)",
        "BIT 1,A",
        "BIT 2,B",
        "BIT 2,C",
        "BIT 2,D",
        "BIT 2,E",
        "BIT 2,H",
        "BIT 2,L",
        "BIT 2,(HL)",
        "BIT 2,A",
        "BIT 3,B",
        "BIT 3,C",
        "BIT 3,D",
        "BIT 3,E",
        "BIT 3,H",
        "BIT 3,L",
        "BIT 3,(HL)",
        "BIT 3,A",
        "BIT 4,B",
        "BIT 4,C",
        "BIT 4,D",
        "BIT 4,E",
        "BIT 4,H",
        "BIT 4,L",
        "BIT 4,(HL)",
        "BIT 4,A",
        "BIT 5,B",
        "BIT 5,C",
        "BIT 5,D",
        "BIT 5,E",
        "BIT 5,H",
        "BIT 5,L",
        "BIT 5,(HL)",
        "BIT 5,A",
        "BIT 6,B",
        "BIT 6,C",
        "BIT 6,D",
        "BIT 6,E",
        "BIT 6,H",
        "BIT 6,L",
        "BIT 6,(HL)",
        "BIT 6,A",
        "BIT 7,B",
        "BIT 7,C",
        "BIT 7,D",
        "BIT 7,E",
        "BIT 7,H",
        "BIT 7,L",
        "BIT 7,(HL)",
        "BIT 7,A",
        "RES 0,B",
        "RES 0,C",
        "RES 0,D",
        "RES 0,E",
        "RES 0,H",
        "RES 0,L",
        "RES 0,(HL)",
        "RES 0,A",
        "RES 1,B",
        "RES 1,C",
        "RES 1,D",
        "RES 1,E",
        "RES 1,H",
        "RES 1,L",
        "RES 1,(HL)",
        "RES 1,A",
        "RES 2,B",
        "RES 2,C",
        "RES 2,D",
        "RES 2,E",
        "RES 2,H",
        "RES 2,L",
        "RES 2,(HL)",
        "RES 2,A",
        "RES 3,B",
        "RES 3,C",
        "RES 3,D",
        "RES 3,E",
        "RES 3,H",
        "RES 3,L",
        "RES 3,(HL)",
        "RES 3,A",
        "RES 4,B",
        "RES 4,C",
        "RES 4,D",
        "RES 4,E",
        "RES 4,H",
        "RES 4,L",
        "RES 4,(HL)",
        "RES 4,A",
        "RES 5,B",
        "RES 5,C",
        "RES 5,D",
        "RES 5,E",
        "RES 5,H",
        "RES 5,L",
        "RES 5,(HL)",
        "RES 5,A",
        "RES 6,B",
        "RES 6,C",
        "RES 6,D",
        "RES 6,E",
        "RES 6,H",
        "RES 6,L",
        "RES 6,(HL)",
        "RES 6,A",
        "RES 7,B",
        "RES 7,C",
        "RES 7,D",
        "RES 7,E",
        "RES 7,H",
        "RES 7,L",
        "RES 7,(HL)",
        "RES 7,A",
        "SET 0,B",
        "SET 0,C",
        "SET 0,D",
        "SET 0,E",
        "SET 0,H",
        "SET 0,L",
        "SET 0,(HL)",
        "SET 0,A",
        "SET 1,B",
        "SET 1,C",
        "SET 1,D",
        "SET 1,E",
        "SET 1,H",
        "SET 1,L",
        "SET 1,(HL)",
        "SET 1,A",
        "SET 2,B",
        "SET 2,C",
        "SET 2,D",
        "SET 2,E",
        "SET 2,H",
        "SET 2,L",
        "SET 2,(HL)",
        "SET 2,A",
        "SET 3,B",
        "SET 3,C",
        "SET 3,D",
        "SET 3,E",
        "SET 3,H",
        "SET 3,L",
        "SET 3,(HL)",
        "SET 3,A",
        "SET 4,B",
        "SET 4,C",
        "SET 4,D",
        "SET 4,E",
        "SET 4,H",
        "SET 4,L",
        "SET 4,(HL)",
        "SET 4,A",
        "SET 5,B",
        "SET 5,C",
        "SET 5,D",
        "SET 5,E",
        "SET 5,H",
        "SET 5,L",
        "SET 5,(HL)",
        "SET 5,A",
        "SET 6,B",
        "SET 6,C",
        "SET 6,D",
        "SET 6,E",
        "SET 6,H",
        "SET 6,L",
        "SET 6,(HL)",
        "SET 6,A",
        "SET 7,B",
        "SET 7,C",
        "SET 7,D",
        "SET 7,E",
        "SET 7,H",
        "SET 7,L",
        "SET 7,(HL)",
        "SET 7,A",
        };
    return CpuCommands[Opcode];
}

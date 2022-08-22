#
# License: See LICENSE.md file
# GitHub: https://github.com/Baekalfen/PyBoy
#

import re
from html.parser import HTMLParser
from urllib.request import urlopen

destination = "../UnrealBoyOpcodes.cpp"

warning = """
/*
* THIS FILE IS AUTO-GENERATED!!!
* DO NOT MODIFY THIS FILE.
* CHANGES TO THE CODE SHOULD BE MADE IN 'opcodes_gen.py'.
*/
"""

default_code = """

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

"""



def inline_signed_int8(arg):
    return "(({} ^ 0x80) - 0x80)".format(arg)


opcodes = []


class MyHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)

        self.counter = 0
        self.tagstack = []

        self.cell_lines = []
        self.stop = False
        self._attrs = None
        self.founddata = False

    def handle_starttag(self, tag, attrs):
        if tag != "br":
            self.founddata = False
            self._attrs = attrs
            self.tagstack.append(tag)

    def handle_endtag(self, tag):
        if not self.founddata and self.tagstack[-1] == "td" and self.counter % 0x100 != 0:
            self.counter += 1
            opcodes.append(None) # Blank operations
        self.tagstack.pop()

    def handle_data(self, data):
        if self.stop or len(self.tagstack) == 0:
            return

        self.founddata = True

        if self.tagstack[-1] == "td":
            self.cell_lines.append(data)

            if len(self.cell_lines) == 4:
                opcodes.append(
                    self.make_opcode(
                        self.cell_lines, ("bgcolor", "#ccffcc") in self._attrs or ("bgcolor", "#ffcccc") in self._attrs
                    )
                )
                self.counter += 1
                self.cell_lines = []

        if self.counter == 0x200:
            self.stop = True

    def make_opcode(self, lines, bit16):
        opcode = self.counter
        flags = lines.pop()
        cycles = lines.pop()
        length = lines.pop()
        name = lines.pop()

        return OpcodeData(opcode, name, length, cycles, bit16, *flags.split())


class Operand:
    def __init__(self, operand):
        self.postoperation = None
        self.pointer = False
        self.highpointer = False
        self.immediate = False
        self.signed = False
        self.is16bit = False
        self.flag = False
        self.operand = operand
        self.codegen(False)

    @property
    def set(self):
        return self.codegen(True)

    @property
    def get(self):
        return self.codegen(False)

    def codegen(self, assign, operand=None):
        if operand is None:
            operand = self.operand

        if operand == "(C)":
            self.highpointer = True
            if assign:
                return "CPU.Motherboard.WriteMemory(0xFF00 + CPU.C, %s)"
            else:
                return "CPU.Motherboard.ReadMemory(0xFF00 + CPU.C)"

        elif operand == "SP+r8":
            self.immediate = True
            self.signed = True

            # post operation set in LD handler!
            return "CPU.SP + " + inline_signed_int8("Value")

        elif operand.startswith("(") and operand.endswith(")"):
            self.pointer = True
            if assign:
                code = "CPU.Motherboard.WriteMemory(%s" % self.codegen(
                    False, operand=re.search(r"\(([a-zA-Z]+\d*)[\+-]?\)", operand).group(1)
                ) + ", %s)"
            else:
                code = "CPU.Motherboard.ReadMemory(%s)" % self.codegen(
                    False, operand=re.search(r"\(([a-zA-Z]+\d*)[\+-]?\)", operand).group(1)
                )

            if "-" in operand or "+" in operand:
                # TODO: Replace with opcode 23 (INC HL)?
                self.postoperation = "CPU.HL %s= 1" % operand[-2]

            return code

        # Sadly, there is an overlap between the register 'C' and to
        # check for the carry flag 'C'.
        elif operand in ["A", "F", "B", "C", "D", "E", "SP", "PC", "HL"]:
            if assign:
                return "CPU." + operand + " = %s"
            else:
                return "CPU." + operand

        elif operand == "H":
            if assign:
                return "CPU.HL = (CPU.HL & 0x00FF) | (%s << 8)"
            else:
                return "(CPU.HL >> 8)"

        elif operand == "L":
            if assign:
                return "CPU.HL = (CPU.HL & 0xFF00) | (%s & 0xFF)"
            else:
                return "(CPU.HL & 0xFF)"

        elif operand in ["AF", "BC", "DE"]:
            if assign:
                return "CPU.SetRegister" + operand.upper() + "(%s)"
            else:
                return "((CPU." + operand[0] + " << 8) + CPU." + operand[1] + ")"

        elif operand in ["Z", "C", "NZ", "NC"]: # flags
            assert not assign
            self.flag = True
            return "CPU.GetFlag" + operand.upper() + "()"

        elif operand in ["d8", "d16", "a8", "a16", "r8"]:
            assert not assign
            code = "Value"
            self.immediate = True

            if operand == "r8":
                code = inline_signed_int8(code)
                self.signed = True
            elif operand == "a8":
                code += " + 0xFF00"
                self.highpointer = True
            return code

        else:
            raise ValueError("Didn'Temp match symbol: %s" % operand)


class Literal:
    def __init__(self, value):
        if isinstance(value, str) and value.find("H") > 0:
            self.value = int(value[:-1], 16)
        else:
            self.value = value
        self.code = str(self.value)
        self.immediate = False

    @property
    def get(self):
        return self.code


class Code:
    def __init__(self, function_name, opcode, name, takes_immediate, length, cycles, branch_op=False):
        self.function_name = function_name
        self.opcode = opcode
        self.name = name
        self.cycles = cycles
        self.takes_immediate = takes_immediate
        self.length = length
        self.lines = []
        self.branch_op = branch_op

    def addline(self, line):
        self.lines.append(line)

    def addlines(self, lines):
        for l in lines:
            self.lines.append(l)

    def getcode(self):
        code = ""
        code += [
            "uint8 FUnrealBoyOpcodes::%s_%0.2X() const // %0.2X %s" % (self.function_name, self.opcode, self.opcode, self.name),
            "uint8 FUnrealBoyOpcodes::%s_%0.2X() const // %0.2X %s" % (self.function_name, self.opcode, self.opcode, self.name)
        ][self.takes_immediate]
        code += "\n{\n\t"

        if not self.branch_op:
            self.lines.append("CPU.PC += %d" % self.length)
            # self.lines.append("CPU.PC &= 0xFFFF")
            self.lines.append("return " + self.cycles[0]) # Choose the 0th cycle count

        # filter line
        index = -1
        for line in self.lines:
            index += 1
            if line.startswith("if"):
                continue
            if line.endswith("else") or line.endswith("{") or line.endswith("}"):
                continue
            self.lines[index] += ";" 


        code += "\n\t".join(self.lines)
        code += "\n}"

        return ("", code)


class OpcodeData:
    def __init__(self, opcode, name, length, cycles, bit16, flag_z, flag_n, flag_h, flag_c):
        self.opcode = opcode
        self.name = name
        self.length = int(length)
        self.cycles = tuple(cycles.split("/"))
        self.flag_z = flag_z
        self.flag_n = flag_n
        self.flag_h = flag_h
        self.flag_c = flag_c
        self.flags = tuple(enumerate([self.flag_c, self.flag_h, self.flag_n, self.flag_z]))
        self.is16bit = bit16

        # TODO: There's no need for this to be so explicit
        # yapf: disable
        self.functionhandlers = {
            "NOP"    : self.NOP,
            "HALT"   : self.HALT,
            "PREFIX" : self.CB,
            "EI"     : self.EI,
            "DI"     : self.DI,
            "STOP"   : self.STOP,
            "LD"     : self.LD,
            "LDH"    : self.LDH,
            "ADD"    : self.ADD,
            "SUB"    : self.SUB,
            "INC"    : self.INC,
            "DEC"    : self.DEC,
            "ADC"    : self.ADC,
            "SBC"    : self.SBC,
            "AND"    : self.AND,
            "OR"     : self.OR,
            "XOR"    : self.XOR,
            "CP"     : self.CP,
            "PUSH"   : self.PUSH,
            "POP"    : self.POP,
            "JP"     : self.JP,
            "JR"     : self.JR,
            "CALL"   : self.CALL,
            "RET"    : self.RET,
            "RETI"   : self.RETI,
            "RST"    : self.RST,
            "DAA"    : self.DAA,
            "SCF"    : self.SCF,
            "CCF"    : self.CCF,
            "CPL"    : self.CPL,
            "RLA"    : self.RLA,
            "RLCA"   : self.RLCA,
            "RLC"    : self.RLC,
            "RL"     : self.RL,
            "RRA"    : self.RRA,
            "RRCA"   : self.RRCA,
            "RRC"    : self.RRC,
            "RR"     : self.RR,
            "SLA"    : self.SLA,
            "SRA"    : self.SRA,
            "SWAP"   : self.SWAP,
            "SRL"    : self.SRL,
            "BIT"    : self.BIT,
            "RES"    : self.RES,
            "SET"    : self.SET,
        }
        # yapf: enable

    def createfunction(self):
        text = self.functionhandlers[self.name.split()[0]]()
        # Compensate for CB operations being "2 bytes long"
        if self.opcode > 0xFF:
            self.length -= 1
        return (self.length, "%s_%0.2X" % (self.name.split()[0], self.opcode), self.name), text

    # Special carry and half-carry for E8 and F8:
    # http://forums.nesdev.com/viewtopic.php?p=42138
    # Blargg: "Both of these set carry and half-carry based on the low
    # byte of SP added to the UNSIGNED immediate byte. The Negative
    # and Zero flags are always cleared. They also calculate SP +
    # SIGNED immediate byte and put the result into SP or HL,
    # respectively."
    def handleflags16bit_E8_F8(self, r0, r1, op, carry=False):
        flagmask = sum(map(lambda nf: (nf[1] == "-") << (nf[0] + 4), self.flags))

        # Only in case we do a dynamic operation, do we include the
        # following calculations
        if flagmask == 0b11110000:
            return ["// No flag operations"]

        lines = []
        # Sets the flags that always get set by operation
        lines.append("uint8 Flag = " + format(sum(map(lambda nf: (nf[1] == "1") << (nf[0] + 4), self.flags)), "#010b"))

        # flag += (((CPU.SP & 0xF) + (Value & 0xF)) > 0xF) << FLAG_H
        if self.flag_h == "H":
            c = " %s CPU.GetFlagC()" % op if carry else ""
            lines.append("Flag += (((%s & 0xF) %s (%s & 0xF)%s) > 0xF) << FLAG_H" % (r0, op, r1, c))

        # flag += (((CPU.SP & 0xFF) + (Value & 0xFF)) > 0xFF) << FLAG_C
        if self.flag_c == "C":
            lines.append("Flag += (((%s & 0xFF) %s (%s & 0xFF)%s) > 0xFF) << FLAG_C" % (r0, op, r1, c))

        # Clears all flags affected by the operation
        lines.append("CPU.F &= " + format(flagmask, "#010b"))
        lines.append("CPU.F |= Flag")
        return lines

    def handleflags16bit(self, r0, r1, op, carry=False):
        flagmask = sum(map(lambda nf: (nf[1] == "-") << (nf[0] + 4), self.flags))

        # Only in case we do a dynamic operation, do we include the
        # following calculations
        if flagmask == 0b11110000:
            return ["// No flag operations"]

        lines = []
        # Sets the ones that always get set by operation
        lines.append("uint8 Flag = " + format(sum(map(lambda nf: (nf[1] == "1") << (nf[0] + 4), self.flags)), "#010b"))

        if self.flag_h == "H":
            c = " %s CPU.GetFlagC()" % op if carry else ""
            lines.append("Flag += (((%s & 0xFFF) %s (%s & 0xFFF)%s) > 0xFFF) << FLAG_H" % (r0, op, r1, c))

        if self.flag_c == "C":
            lines.append("Flag += (Temp > 0xFFFF) << FLAG_C")

        # Clears all flags affected by the operation
        lines.append("CPU.F &= " + format(flagmask, "#010b"))
        lines.append("CPU.F |= Flag")
        return lines

    def handleflags8bit(self, r0, r1, op, carry=False):
        flagmask = sum(map(lambda nf: (nf[1] == "-") << (nf[0] + 4), self.flags))

        # Only in case we do a dynamic operation, do we include the
        # following calculations
        if flagmask == 0b11110000:
            return ["// No flag operations"]

        lines = []
        # Sets the ones that always get set by operation
        lines.append("uint8 Flag = " + format(sum(map(lambda nf: (nf[1] == "1") << (nf[0] + 4), self.flags)), "#010b"))

        if self.flag_z == "Z":
            lines.append("Flag += ((Temp & 0xFF) == 0) << FLAG_Z")

        if self.flag_h == "H" and op == "-":
            c = " %s CPU.GetFlagC()" % op if carry else ""
            lines.append("Flag += (((%s & 0xF) %s (%s & 0xF)%s) < 0) << FLAG_H" % (r0, op, r1, c))
        elif self.flag_h == "H":
            c = " %s CPU.GetFlagC()" % op if carry else ""
            lines.append("Flag += (((%s & 0xF) %s (%s & 0xF)%s) > 0xF) << FLAG_H" % (r0, op, r1, c))

        if self.flag_c == "C" and op == "-":
            lines.append("Flag += (Temp < 0) << FLAG_C")
        elif self.flag_c == "C":
            lines.append("Flag += (Temp > 0xFF) << FLAG_C")

        # Clears all flags affected by the operation
        lines.append("CPU.F &= " + format(flagmask, "#010b"))
        lines.append("CPU.F |= Flag")
        return lines

    ###################################################################
    #
    # MISC OPERATIONS
    #
    def NOP(self):
        code = Code(self.name.split()[0], self.opcode, self.name, 0, self.length, self.cycles)
        return code.getcode()

    def HALT(self):
        code = Code(self.name.split()[0], self.opcode, self.name, 0, self.length, self.cycles, branch_op=True)

        # TODO: Implement HALT bug.
        code.addlines([
            "CPU.bHalted = true",
            "return " + self.cycles[0],
        ])
        return code.getcode()

    def CB(self):
        code = Code(self.name.split()[0], self.opcode, self.name, 0, self.length, self.cycles)
        code.addline('ensureAlwaysMsgf(false, TEXT("CB cannot be called!"))')
        return code.getcode()

    def EI(self):
        code = Code(self.name.split()[0], self.opcode, self.name, 0, self.length, self.cycles)
        code.addline("CPU.bInterruptMasterEnabled = true")
        return code.getcode()

    def DI(self):
        code = Code(self.name.split()[0], self.opcode, self.name, 0, self.length, self.cycles)
        code.addline("CPU.bInterruptMasterEnabled = false")
        return code.getcode()

    def STOP(self):
        code = Code(self.name.split()[0], self.opcode, self.name, True, self.length, self.cycles)
        # code.addline("pass")
        # code.addLine("raise Exception('STOP not implemented!')")
        return code.getcode()

    def DAA(self):
        left = Operand("A")
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)

        # http://stackoverflow.com/a/29990058/3831206
        # http://forums.nesdev.com/viewtopic.php?Temp=9088
        code.addlines([
            "uint8 Temp = %s" % left.get,
            "uint8 Corr = 0",
            "Corr |= CPU.GetFlagH() ? 0x06 : 0x00",
            "Corr |= CPU.GetFlagC() ? 0x60 : 0x00",
            "if (CPU.GetFlagN())",
            "{",
            "\tTemp -= Corr",
            "}",
            "else",
            "{",
            "\tCorr |= (Temp & 0x0F) > 0x09 ? 0x06 : 0x00",
            "\tCorr |= Temp > 0x99 ? 0x60 : 0x00",
            "\tTemp += Corr",
            "}",
            "uint8 Flag = 0",
            "Flag += ((Temp & 0xFF) == 0) << FLAG_Z",
            "Flag += ((Corr & 0x60) != 0) << FLAG_C",
            "CPU.F &= 0b01000000",
            "CPU.F |= Flag",
            "Temp &= 0xFF",
            left.set % "Temp",
        ])
        return code.getcode()

    def SCF(self):
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addlines(self.handleflags8bit(None, None, None))
        return code.getcode()

    def CCF(self):
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addlines([
            "uint8 Flag = (CPU.F & 0b00010000) ^ 0b00010000",
            "CPU.F &= 0b10000000",
            "CPU.F |= Flag",
        ])
        return code.getcode()

    def CPL(self):
        left = Operand("A")
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addline(left.set % ("(~%s) & 0xFF" % left.get))
        code.addlines(self.handleflags8bit(None, None, None))
        return code.getcode()

    ###################################################################
    #
    # LOAD OPERATIONS
    #
    def LD(self):
        r0, r1 = self.name.split()[1].split(",")
        left = Operand(r0)
        right = Operand(r1)

        # FIX: There seems to be a wrong opcode length on E2 and F2
        if self.opcode == 0xE2 or self.opcode == 0xF2:
            self.length = 1

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        if self.is16bit and left.immediate and left.pointer:
            code.addline(left.set % ("%s & 0xFF" % right.get))
            a, b = left.set.split(",")
            code.addline((a+"+1,"+b) % ("%s >> 8" % right.get))
        else:
            # Special handling of AF, BC, DE
            # print(left.set, right.get, hex(self.opcode))
            code.addline(left.set % right.get)

        # Special HL-only operations
        if left.postoperation is not None:
            code.addline(left.postoperation)
            code.addline("CPU.HL &= 0xFFFF")
        elif right.postoperation is not None:
            code.addline(right.postoperation)
            code.addline("CPU.HL &= 0xFFFF")
        elif self.opcode == 0xF8:
            # E8 and F8 http://forums.nesdev.com/viewtopic.php?p=42138
            code.addline("uint16 Temp = CPU.HL")
            code.addlines(self.handleflags16bit_E8_F8("CPU.SP", "Value", "+", False))
            code.addline("CPU.HL &= 0xFFFF")

        return code.getcode()

    def LDH(self):
        return self.LD()

    ###################################################################
    #
    # ALU OPERATIONS
    #
    def ALU(self, left, right, op, carry=False):
        lines = []

        left.assign = False
        right.assign = False
        calc = " ".join(["auto Temp", "=", left.get, op, right.get])

        if carry:
            calc += " " + op + " CPU.GetFlagC()"

        lines.append(calc)

        if self.opcode == 0xE8:
            # E8 and F8 http://forums.nesdev.com/viewtopic.php?p=42138
            lines.extend(self.handleflags16bit_E8_F8(left.get, "Value", op, carry))
            lines.append("Temp &= 0xFFFF")
        elif self.is16bit:
            lines.extend(self.handleflags16bit(left.get, right.get, op, carry))
            lines.append("Temp &= 0xFFFF")
        else:
            lines.extend(self.handleflags8bit(left.get, right.get, op, carry))
            lines.append("Temp &= 0xFF")

        # HAS TO BE THE LAST INSTRUCTION BECAUSE OF CP!
        lines.append(left.set % "Temp")
        return lines

    def ADD(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = Operand("A")
            right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "+"))
        return code.getcode()

    def SUB(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = Operand("A")
            right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "-"))
        return code.getcode()

    def INC(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        right = Literal(1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "+"))
        return code.getcode()

    def DEC(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        right = Literal(1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "-"))
        return code.getcode()

    def ADC(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = Operand("A")
            right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "+", carry=True))
        return code.getcode()

    def SBC(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = Operand("A")
            right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "-", carry=True))
        return code.getcode()

    def AND(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = Operand("A")
            right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "&"))
        return code.getcode()

    def OR(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = Operand("A")
            right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "|"))
        return code.getcode()

    def XOR(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = Operand("A")
            right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        code.addlines(self.ALU(left, right, "^"))
        return code.getcode()

    def CP(self):
        r1 = self.name.split()[1]
        left = Operand("A")
        right = Operand(r1)

        code = Code(
            self.name.split()[0], self.opcode, self.name, left.immediate or right.immediate, self.length, self.cycles
        )
        # CP is equal to SUB, but without saving the result.
        # Therefore; we discard the last instruction.
        code.addlines(self.ALU(left, right, "-")[:-1])
        return code.getcode()

    ###################################################################
    #
    # PUSH/POP OPERATIONS
    #
    def PUSH(self):
        r0 = self.name.split()[1]
        left = Operand(r0)

        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        if "HL" in left.get:
            code.addlines([
                "CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.HL >> 8); // High",
                "CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.HL & 0xFF); // Low",
                "CPU.SP -= 2",
                "CPU.SP &= 0xFFFF",
            ])
        else:
            # A bit of a hack, but you can only push double registers
            code.addline("CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.%s); // High" % left.operand[-2])
            if left.operand == "AF":
                # by taking fx 'A' and 'F' directly, we save calculations
                code.addline("CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.%s & 0xF0); // Low" % left.operand[-1])
            else:
                # by taking fx 'A' and 'F' directly, we save calculations
                code.addline("CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.%s); // Low" % left.operand[-1])
            code.addline("CPU.SP -= 2")
            code.addline("CPU.SP &= 0xFFFF")

        return code.getcode()

    def POP(self):
        r0 = self.name.split()[1]
        left = Operand(r0)

        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        if "HL" in left.get:
            code.addlines([
                (left.set % "(CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8) + "
                 "CPU.Motherboard.ReadMemory(CPU.SP)") + "; // High",
                "CPU.SP += 2",
                "CPU.SP &= 0xFFFF",
            ])
        else:
            if left.operand.endswith("F"): # Catching AF
                fmask = " & 0xF0"
            else:
                fmask = ""
            # See comment from PUSH
            code.addline("CPU.%s = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF); // High" % left.operand[-2])
            if left.operand == "AF":
                code.addline("CPU.%s = CPU.Motherboard.ReadMemory(CPU.SP)%s & 0xF0; // Low" % (left.operand[-1], fmask))
            else:
                code.addline("CPU.%s = CPU.Motherboard.ReadMemory(CPU.SP)%s; // Low" % (left.operand[-1], fmask))
            code.addline("CPU.SP += 2")
            code.addline("CPU.SP &= 0xFFFF")

        return code.getcode()

    ###################################################################
    #
    # CONTROL FLOW OPERATIONS
    #
    def JP(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = None
            right = Operand(r1)

        r_code = right.get
        if left is not None:
            l_code = left.get
            if l_code.endswith("C") and "NC" not in l_code:
                left.flag = True
                l_code = "CPU.GetFlagC()"
            assert left.flag
        elif right.pointer:
            # FIX: Wrongful syntax of "JP (HL)" actually meaning "JP HL"
            right.pointer = False
            r_code = right.codegen(False, operand="HL")
        else:
            assert right.immediate

        code = Code(
            self.name.split()[0], self.opcode, self.name, right.immediate, self.length, self.cycles, branch_op=True
        )
        if left is None:
            code.addlines(["CPU.PC = %s" % ("Value" if right.immediate else r_code), "return " + self.cycles[0]])
        else:
            code.addlines([
                "if (%s)" % l_code,
                "{",
                "\tCPU.PC = %s" % ("Value" if right.immediate else r_code),
                "\treturn " + self.cycles[0],
                "}",
                "else",
                "{",
                "\tCPU.PC += %s" % self.length,
                "\tCPU.PC &= 0xFFFF",
                "\treturn " + self.cycles[1],
                "}"
            ])

        return code.getcode()

    def JR(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = None
            right = Operand(r1)

        if left is not None:
            l_code = left.get
            if l_code.endswith("C") and "NC" not in l_code:
                left.flag = True
                l_code = "CPU.GetFlagC()"
            assert left.flag
        assert right.immediate

        code = Code(
            self.name.split()[0], self.opcode, self.name, right.immediate, self.length, self.cycles, branch_op=True
        )
        if left is None:
            code.addlines([
                "CPU.PC += %d + " % self.length + inline_signed_int8("Value"),
                "CPU.PC &= 0xFFFF",
                "return " + self.cycles[0],
            ])
        else:
            code.addlines([
                "CPU.PC += %d" % self.length,
                "if (%s)" % l_code,
                "{",
                "\tCPU.PC += " + inline_signed_int8("Value"),
                "\tCPU.PC &= 0xFFFF",
                "\treturn " + self.cycles[0],
                "}",
                "else",
                "{",
                "\tCPU.PC &= 0xFFFF",
                "\treturn " + self.cycles[1],
                "}",
            ])

        return code.getcode()

    def CALL(self):
        if self.name.find(",") > 0:
            r0, r1 = self.name.split()[1].split(",")
            left = Operand(r0)
            right = Operand(r1)
        else:
            r1 = self.name.split()[1]
            left = None
            right = Operand(r1)

        if left is not None:
            l_code = left.get
            if l_code.endswith("C") and "NC" not in l_code:
                left.flag = True
                l_code = "CPU.GetFlagC()"
            assert left.flag
        assert right.immediate

        code = Code(
            self.name.split()[0], self.opcode, self.name, right.immediate, self.length, self.cycles, branch_op=True
        )

        # Taken from PUSH
        code.addlines([
            "CPU.PC += %s" % self.length,
            "CPU.PC &= 0xFFFF",
        ])

        if left is None:
            code.addlines([
                "CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High",
                "CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low",
                "CPU.SP -= 2",
                "CPU.SP &= 0xFFFF",
                "CPU.PC = %s" % ("Value" if right.immediate else right.get),
                "return " + self.cycles[0],
            ])
        else:
            code.addlines([
                "if (%s)" % l_code,
                "{",
                "\tCPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High",
                "\tCPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low",
                "\tCPU.SP -= 2",
                "\tCPU.SP &= 0xFFFF",
                "\tCPU.PC = %s" % ("Value" if right.immediate else right.get),
                "\treturn " + self.cycles[0],
                "}",
                "else",
                "{",
                "\treturn " + self.cycles[1],
                "}",
            ])

        return code.getcode()

    def RET(self):
        if self.name == "RET":
            left = None
        else:
            r0 = self.name.split()[1]
            left = Operand(r0)

            l_code = left.get
            if left is not None:
                if l_code.endswith("C") and "NC" not in l_code:
                    left.flag = True
                    l_code = "CPU.GetFlagC()"
                assert left.flag

        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles, branch_op=True)
        if left is None:
            code.addlines([
                "CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High",
                "CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low",
                "CPU.SP += 2",
                "CPU.SP &= 0xFFFF",
                "return " + self.cycles[0],
            ])
        else:
            code.addlines([
                "if (%s)" % l_code,
                "{",
                "\tCPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High",
                "\tCPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low",
                "\tCPU.SP += 2",
                "\tCPU.SP &= 0xFFFF",
                "\treturn " + self.cycles[0],
                "}",
                "else",
                "{",
                "\tCPU.PC += %s" % self.length,
                "\tCPU.PC &= 0xFFFF",
                "\treturn " + self.cycles[1],
                "}",
            ])

        return code.getcode()

    def RETI(self):
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles, branch_op=True)
        code.addline("CPU.bInterruptMasterEnabled = true")
        code.addlines([
            "CPU.PC = CPU.Motherboard.ReadMemory((CPU.SP + 1) & 0xFFFF) << 8; // High",
            "CPU.PC |= CPU.Motherboard.ReadMemory(CPU.SP); // Low",
            "CPU.SP += 2",
            "CPU.SP &= 0xFFFF",
            "return " + self.cycles[0],
        ])

        return code.getcode()

    def RST(self):
        r1 = self.name.split()[1]
        right = Literal(r1)

        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles, branch_op=True)

        # Taken from PUSH and CALL
        code.addlines([
            "CPU.PC += %s" % self.length,
            "CPU.PC &= 0xFFFF",
            "CPU.Motherboard.WriteMemory((CPU.SP-1) & 0xFFFF, CPU.PC >> 8); // High",
            "CPU.Motherboard.WriteMemory((CPU.SP-2) & 0xFFFF, CPU.PC & 0xFF); // Low",
            "CPU.SP -= 2",
            "CPU.SP &= 0xFFFF",
        ])

        code.addlines([
            "CPU.PC = %s" % (right.code),
            "return " + self.cycles[0],
        ])

        return code.getcode()

    ###################################################################
    #
    # ROTATE/SHIFT OPERATIONS
    #
    def rotateleft(self, name, left, throughcarry=False):
        code = Code(name, self.opcode, self.name, False, self.length, self.cycles)
        left.assign = False
        if throughcarry:
            code.addline(("uint16 Temp = (%s << 1)" % left.get) + " + CPU.GetFlagC()")
        else:
            code.addline("uint16 Temp = (%s << 1) + (%s >> 7)" % (left.get, left.get))
        code.addlines(self.handleflags8bit(left.get, None, None, throughcarry))
        code.addline("Temp &= 0xFF")
        left.assign = True
        code.addline(left.set % "Temp")
        return code

    def RLA(self):
        left = Operand("A")
        code = self.rotateleft(self.name.split()[0], left, throughcarry=True)
        return code.getcode()

    def RLCA(self):
        left = Operand("A")
        code = self.rotateleft(self.name.split()[0], left)
        return code.getcode()

    def RLC(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        code = self.rotateleft(self.name.split()[0], left)
        return code.getcode()

    def RL(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        code = self.rotateleft(self.name.split()[0], left, throughcarry=True)
        return code.getcode()

    def rotateright(self, name, left, throughcarry=False):
        code = Code(name, self.opcode, self.name, False, self.length, self.cycles)
        left.assign = False
        if throughcarry:
            # Trigger "overflow" for carry flag
            code.addline(("uint16 Temp = (%s >> 1)" % left.get) + " + (CPU.GetFlagC() << 7)" + " + ((%s & 1) << 8)" % (left.get))
        else:
            # Trigger "overflow" for carry flag
            code.addline("uint16 Temp = (%s >> 1) + ((%s & 1) << 7)" % (left.get, left.get) + " + ((%s & 1) << 8)" % (left.get))
        code.addlines(self.handleflags8bit(left.get, None, None, throughcarry))
        code.addline("Temp &= 0xFF")
        code.addline(left.set % "Temp")
        return code

    def RRA(self):
        left = Operand("A")
        code = self.rotateright(self.name.split()[0], left, throughcarry=True)
        return code.getcode()

    def RRCA(self):
        left = Operand("A")
        code = self.rotateright(self.name.split()[0], left)
        return code.getcode()

    def RRC(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        code = self.rotateright(self.name.split()[0], left)
        return code.getcode()

    def RR(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        code = self.rotateright(self.name.split()[0], left, throughcarry=True)
        return code.getcode()

    def SLA(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addline("uint16 Temp = (%s << 1)" % left.get)
        code.addlines(self.handleflags8bit(left.get, None, None, False))
        code.addline("Temp &= 0xFF")
        code.addline(left.set % "Temp")
        return code.getcode()

    def SRA(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        # FIX: All documentation tells it should have carry enabled
        self.flag_c = "C"
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        # Actual shift / MSB unchanged / Trigger "overflow" for carry flag
        code.addline("uint16 Temp = ((%s >> 1) | (%s & 0x80)) + ((%s & 1) << 8)" % (left.get, left.get, left.get))
        code.addlines(self.handleflags8bit(left.get, None, None, False))
        code.addline("Temp &= 0xFF")
        code.addline(left.set % "Temp")
        return code.getcode()

    def SRL(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        #              Actual shift / Trigger "overflow" for carry flag
        code.addline("uint16 Temp = (%s >> 1) + ((%s & 1) << 8)" % (left.get, left.get))
        code.addlines(self.handleflags8bit(left.get, None, None, False))
        code.addline("Temp &= 0xFF")
        code.addline(left.set % "Temp")
        return code.getcode()

    def SWAP(self):
        r0 = self.name.split()[1]
        left = Operand(r0)
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addline("uint16 Temp = ((%s & 0xF0) >> 4) | ((%s & 0x0F) << 4)" % (left.get, left.get))
        code.addlines(self.handleflags8bit(left.get, None, None, False))
        code.addline("Temp &= 0xFF")
        code.addline(left.set % "Temp")
        return code.getcode()

    ###################################################################
    #
    # BIT OPERATIONS
    #
    def BIT(self):
        r0, r1 = self.name.split()[1].split(",")
        left = Literal(r0)
        right = Operand(r1)
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addline("uint16 Temp = %s & (1 << %s)" % (right.get, left.get))
        code.addlines(self.handleflags8bit(left.get, right.get, None, False))

        return code.getcode()

    def RES(self):
        r0, r1 = self.name.split()[1].split(",")
        left = Literal(r0)
        right = Operand(r1)

        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addline("uint16 Temp = %s & ~(1 << %s)" % (right.get, left.get))
        code.addline(right.set % "Temp")
        return code.getcode()

    def SET(self):
        r0, r1 = self.name.split()[1].split(",")
        left = Literal(r0)
        right = Operand(r1)
        code = Code(self.name.split()[0], self.opcode, self.name, False, self.length, self.cycles)
        code.addline("uint16 Temp = %s | (1 << %s)" % (right.get, left.get))
        code.addline(right.set % "Temp")
        return code.getcode()


def update():
    # response = urlopen("http://pastraiser.com/cpu/gameboy/gameboy_opcodes.html")
    # html = response.read().replace(b"&nbsp;", b"<br>").decode()
    with open("gameboy_opcodes.html", "r") as file:
        html = file.read() 

    parser = MyHTMLParser()
    parser.feed(html)

    opcodefunctions = map(lambda x: (None, None) if x is None else x.createfunction(), opcodes)

    with open(destination, "w") as f:
        f.write(warning)
        f.write(default_code)
        lookuplist = []
        for lookuptuple, code in opcodefunctions:
            lookuplist.append(lookuptuple)

            if code is None:
                continue

            (pxd, functiontext) = code

            # breakpoint()
            f.write(functiontext.replace("\t", " " * 4) + "\n\n")

        f.write("uint8 FUnrealBoyOpcodes::NoOpcode()\n{\n    return 0;\n}\n\n")

        # InitOpcodeTable
        f.write("void FUnrealBoyOpcodes::InitOpcodeTable()\n")
        f.write("{\n")
        f.write(" "*4 + 'OpcodeTable = TArray<FOpcodeFunction> {\n' + " "*8)
        for i, Temp in enumerate(lookuplist):
            Temp = Temp if Temp is not None else (0, "NoOpcode", "")
            Temp = str(Temp[1]).replace("'", "")
            f.write("FOpcodeFunction::CreateRaw(this, &FUnrealBoyOpcodes::" + Temp + "),")
            f.write("\n" + " "*8)
        f.write("};\n")
        f.write("}\n")

        # GetOpcodeLength
        f.write("uint8 FUnrealBoyOpcodes::GetOpcodeLength(uint16 Opcode)\n")
        f.write("{\n")
        f.write(" "*4 + 'static TArray<uint8> OpcodeLengths = TArray<uint8> {\n' + " "*8)
        for i, Temp in enumerate(lookuplist):
            Temp = Temp if Temp is not None else (0, "no_opcode", "")
            f.write(str(Temp[0]).replace("'", "") + ",")
            if (i+1) % 16 == 0:
                f.write("\n" + " "*8)
            else:
                f.write(" ")
        f.write("};\n")
        f.write(" "*4 + "return OpcodeLengths[Opcode];\n")
        f.write("}\n")

        f.write(
            """
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

"""
        )

        indent = 4
        f.write("FString FUnrealBoyOpcodes::GetCommandName(uint16 Opcode)\n")
        f.write("{\n")
        f.write(" "*indent + "static TArray<FString> CpuCommands = TArray<FString> {\n" + " "*indent*2)
        for _, Temp in enumerate(lookuplist):
            Temp = Temp if Temp is not None else (0, "no_opcode", "")
            f.write(f"\"{Temp[2]}\",\n" + " "*indent*2)

        f.write("};\n")
        f.write(" "*4 + "return CpuCommands[Opcode];\n")
        f.write("}\n")

def load():
    # if os.path.exists(destination):
    #     return
    update()


if __name__ == "__main__":
    load()

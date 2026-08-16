#include "6502.hpp"
#include <cstdint>
#include <cstdio>
#include <string>

using namespace std;

void Mem::Initialise() {

    for (u32 i = 0; i < MAX_MEM; i++) {
        Data[i] = 0;
    }

}

Byte Mem::operator[](u32 Address) const {
    return Data[Address];
}

Byte& Mem::operator[](u32 Address) {
    return Data[Address];
}

void Mem::WriteWord(Word Value, u32 Address, u32& Cycles) {

    Data[Address] = Value & 0xFF;
    Data[Address + 1] = Value >> 8;

    Cycles -= 2;
}


void CPU::Reset(Mem& memory) {

    PC = 0xFFFC;
    SP = 0xFF;

    C = Z = I = D = B = V = N = 0;

    A = X = Y = 0;

    memory.Initialise();

}


Byte CPU::FetchByte(Mem& memory) {

    Byte Data = memory[PC];

    PC++;

    return Data;
}

Word CPU::FetchWord(Mem& memory) {

    Word Data = memory[PC];
    PC++;

    Data |= static_cast<Word>(memory[PC]) << 8;
    PC++;

    return Data;
}

Byte CPU::Readbyte(Byte Address, Mem& memory) {

    return memory[Address];

}

void CPU::LDASetStatus() {

    Z = (A == 0);
    N = (A & 0b10000000) > 0;

}

void CPU::LDXSetStatus() {

    Z = (X == 0);
    N = (X & 0b10000000) > 0;

}

void CPU::LDYSetStatus() {

    Z = (Y == 0);
    N = (Y & 0b10000000) > 0;

}

string CPU::GetRegisters() {

    char buffer[512];

    snprintf(
        buffer,
        sizeof(buffer),

        "A:  %02X\n"
        "X:  %02X\n"
        "Y:  %02X\n"
        "PC: %04X\n"
        "SP: %02X\n"
        "C:  %d\n"
        "Z:  %d\n"
        "I:  %d\n"
        "D:  %d\n"
        "B:  %d\n"
        "V:  %d\n"
        "N:  %d\n",

        A, X, Y,
        PC, SP,
        C, Z, I, D, B, V, N
    );

    return string(buffer);
}

void CPU::Execute(u32 Instructions, Mem& memory) {

    while (Instructions > 0) {

        Step(memory);

        Instructions--;

    }

}

void CPU::Step(Mem& memory) {

    Byte Ins = FetchByte(memory);

    switch (Ins) {

        case INS_LDA_IM: {

            Byte Value = FetchByte(memory);

            A = Value;

            LDASetStatus();

        } break;

        case INS_LDA_ZP: {

            Byte ZeroPageAddr = FetchByte(memory);

            A = Readbyte(ZeroPageAddr, memory);

            LDASetStatus();

        } break;

        case INS_LDA_ZPX: {

            Byte ZeroPageAddr = FetchByte(memory);

            ZeroPageAddr += X;

            A = Readbyte(ZeroPageAddr, memory);

            LDASetStatus();

        } break;

        case INS_LDX_IM: {

            Byte Value = FetchByte(memory);

            X = Value;

            LDXSetStatus();

        } break;

        case INS_LDY_IM: {

            Byte Value = FetchByte(memory);

            Y = Value;

            LDYSetStatus();

        } break;

        case INS_INX: {

            X++;

            Z = (X == 0);
            N = (X & 0b10000000) > 0;

        } break;

        case INS_DEX: {

            X--;

            Z = (X == 0);
            N = (X & 0b10000000) > 0;

        } break;

        case INS_INY: {

            Y++;

            Z = (Y == 0);
            N = (Y & 0b10000000) > 0;

        } break;

        case INS_DEY: {

            Y--;

            Z = (Y == 0);
            N = (Y & 0b10000000) > 0;

        } break;

        case INS_STA_ABS: {

            Word Address = FetchWord(memory);
            memory[Address] = A;

        } break;

        case INS_ADC_IM: {

            Byte Value = FetchByte(memory);

            Word Result = A + Value + C;

            C = (Result > 0xFF);

            A = Result & 0xFF;

            Z = (A == 0);
            N = (A & 0b10000000) != 0;

        } break;

        case INS_JMP_ABS: {

            Word Address = FetchWord(memory);

            PC = Address;

        } break;

        case INS_CPX_IM: {

            Byte Value = FetchByte(memory);

            Word Result = X - Value;

            C = (X >= Value);
            Z = ((Result & 0xFF) == 0);
            N = (Result & 0x80) != 0;

        } break;

        case INS_BNE: {

            Byte Offset = FetchByte(memory);

            if (Z == 0) {
                PC += static_cast<int8_t>(Offset);
            }

        } break;

        case INS_BEQ: {

            Byte Offset = FetchByte(memory);

            if (Z == 1) {
                PC += static_cast<int8_t>(Offset);
            }

        } break;

        default: {

            printf("Instruction not handled: %02X\n", Ins);

        } break;

    }

}

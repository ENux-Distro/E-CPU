#include "6502.hpp"
#include "6502-shell.hpp"

#include <iostream>
#include <string>

using namespace std;

int main() {

    Mem mem;
    CPU cpu;

    cpu.Reset(mem);

    // Test program
    mem[0x8000] = CPU::INS_LDX_IM;
    mem[0x8001] = 0x00;

    mem[0x8002] = CPU::INS_INX;

    mem[0x8003] = CPU::INS_CPX_IM;
    mem[0x8004] = 0x05;

    mem[0x8005] = CPU::INS_BNE;
    mem[0x8006] = 0xFB;

    cpu.PC = 0x8000;

    string command;

    while (true) {

        cout << "> ";
        fflush(stdout);

        getline(cin, command);

        if (command == "exit" || command == "quit")
            break;

        cout << ExecuteCommand(cpu, mem, command);
    }

    return 0;
}

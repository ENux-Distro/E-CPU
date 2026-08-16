#include "6502.hpp"
#include "6502-shell.hpp"

#include <string>

static Mem mem;
static CPU cpu;

static std::string output;

extern "C" {

void cpu_init() {
    cpu.Reset(mem);

    mem[0x8000] = CPU::INS_LDX_IM;
    mem[0x8001] = 0x00;

    mem[0x8002] = CPU::INS_INX;

    mem[0x8003] = CPU::INS_CPX_IM;
    mem[0x8004] = 0x05;

    mem[0x8005] = CPU::INS_BNE;
    mem[0x8006] = 0xFB;

    cpu.PC = 0x8000;
}

const char* execute_command(const char* command) {
    output = ExecuteCommand(cpu, mem, command);
    return output.c_str();
}

}

#include "6502-shell.hpp"

#include <cstdio>
#include <string>

using namespace std;

string ExecuteCommand(
    CPU& cpu,
    Mem& mem,
    const string& command
) {

    if (command == "help") {

        return
            "Available commands:\n"
            "  help          Show this help\n"
            "  regs          Show CPU registers\n"
            "  step          Execute one instruction\n"
            "  mem <address> Read memory\n"
            "  exit/quit     Exit emulator\n";
    }

    if (command == "regs") {
        return cpu.GetRegisters();
    }

    if (command == "step") {

        cpu.Step(mem);

        return cpu.GetRegisters();
    }

    if (command.rfind("mem ", 0) == 0) {

        unsigned int Address;

        if (sscanf(command.c_str() + 4, "%x", &Address) == 1) {

            char buffer[64];

            snprintf(
                buffer,
                sizeof(buffer),
                "%04X: %02X\n",
                Address,
                mem[Address]
            );

            return string(buffer);

        }

        return "Usage: mem <address>\n";
    }

    if (command == "exit" || command == "quit") {
        return "";
    }

    return "Unknown command. Type 'help'.\n";
}

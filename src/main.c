#include <stdio.h>
#include <stdint.h>

#include "memory.h"
#include "loader.h"
#include "emulator.h"
#include "debug.h"

int main() {
    init();
    /*
    load_file("test.hex");
    pretty_print_instruction();

    shutdown();
    */

    uint32_t instruction;
    instruction = 0x0c01;

    PC = 0x506;
    write_16bit(memory + PC + 2, (int8_t)-1);
    D(1) = (int8_t)-128;
    cmpi(instruction);

    return 0;
}

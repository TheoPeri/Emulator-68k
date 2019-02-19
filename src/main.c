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
    instruction = 0xd2c8;
    PC = 0x500;
    A(0) = 0x12347878; 
    A(1) = 0x1;
    adda(instruction);

    return 0;
}

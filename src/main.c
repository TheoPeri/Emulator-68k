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
    instruction = 0xd240;
    PC = 0x50c;
    D(0) = 0x1; 
    D(1) = -0x1;
    add(instruction);
    return 0;
}

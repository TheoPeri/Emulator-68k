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
    instruction = 0xb2c0;

    PC = 0x50c;
    D(0) = 0x2; 
    A(1) = 0x1;
    cmpa(instruction);
    

    return 0;
}

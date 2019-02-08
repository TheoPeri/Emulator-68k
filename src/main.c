#include <stdio.h>
#include <stdint.h>

#include "memory.h"
#include "loader.h"
#include "emulator.h"
#include "debug.h"

extern uint8_t *memory;

int main() {
    init();
    
    load_file("test.hex");
    pretty_print_instruction();

    shutdown();

    return 0;
}

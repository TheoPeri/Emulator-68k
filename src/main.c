#include <stdio.h>
#include <stdint.h>

#include "memory.h"
#include "loader.h"
#include "emulator.h"
#include "debug.h"

int main() {
    init_memory();
    load_file("assembler/load_exemple.hex");

    init();

    pretty_print_instruction();

    shutdown();
    return 0;
}

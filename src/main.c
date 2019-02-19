#include <stdio.h>
#include <stdint.h>

#include "memory.h"
#include "loader.h"
#include "emulator.h"
#include "debug.h"

int main() {
    init();
    
    load_file("test.hex");
    pretty_print_instruction();

    shutdown();
<<<<<<< HEAD
    */
    uint32_t instruction;

    instruction = 0x5040;

    PC = 0x500;
    D(0) = 0x32; // 8
    addq(instruction);


=======
   
>>>>>>> movem done
    return 0;
}

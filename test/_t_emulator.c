#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "../src/memory.h"
#include "../src/emulator.h"

void setup_emulator(void) {
    memory = calloc(16777216, 1);
}

Test(emulator, test_rts, .init=setup_emulator) {
    A7 = 0;
    write_32bit(memory, 0x12345678); 

    rts();

    cr_expect(PC == 0x12345678, "Expect PC == 0x12345678");
    cr_expect(A7 == 0x4, "Expect A7 == 0x4");
}

Test(emulator, test_bra, .init=setup_emulator) {
    uint16_t instruction = 0x6100;
    
    // test word
    PC = 0xa;
    
    write_16bit(memory + 12, 0x6789);
    bra(instruction);

    cr_expect(PC == 0x6789 + 0xa, "Expect the valid new address => %x", PC);

    // test byte
    PC = 0xa;
    instruction = 0x6177;
    
    bra(instruction);

    cr_expect(PC == 0x77 + 0xa, "Expect the valid new address => %x", PC);
}

Test(emulator, test_bsr, .init=setup_emulator) {
    uint16_t instruction = 0x6100;
    
    // test word
    A7 = 0x4;
    PC = 0xa;
    
    write_16bit(memory + 12, 0x6789);
    bsr(instruction);

    cr_expect(A7 == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x6789 + 0xa, "Expect the valid new address => %x", PC);
    cr_expect(0xe == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));

    // test byte
    A7 = 0x4;
    PC = 0xa;
    instruction = 0x6177;
    
    bsr(instruction);

    cr_expect(A7 == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x77 + 0xa, "Expect the valid new address => %x", PC);
    cr_expect(0xc == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));
}

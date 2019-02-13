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
    A(7) = 0;
    write_32bit(memory, 0x12345678); 

    rts();

    cr_expect(PC == 0x12345678, "Expect PC == 0x12345678");
    cr_expect(A(7) == 0x4, "Expect A7 == 0x4");
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
    A(7) = 0x4;
    PC = 0xa;
    
    write_16bit(memory + 12, 0x6789);
    bsr(instruction);

    cr_expect(A(7) == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x6789 + 0xa, "Expect the valid new address => %x", PC);
    cr_expect(0xe == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));

    // test byte
    A(7) = 0x4;
    PC = 0xa;
    instruction = 0x6177;
    
    bsr(instruction);

    cr_expect(A(7) == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x77 + 0xa, "Expect the valid new address => %x", PC);
    cr_expect(0xc == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));
}


Test(emulator, test_adda, .init=setup_emulator) {
    uint16_t instruction;

    // test data register .w 
    instruction = 0xd8c2;

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666;
    adda(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0x698, "Error on the destination address for adda: "
        "(data register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666666;
    adda(instruction);
    cr_expect(A(4) == 0x6698, "Error on the destination address for adda: "
        "(data register .w) => A4 = 0x%x", A(4));

    // test data register .l 
    instruction = 0xd9c2;

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666;
    adda(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0x698, "Error on the destination address for adda: "
        "(data register .l) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666666;
    adda(instruction);
    cr_expect(A(4) == 0x666698, "Error on the destination address for adda: "
        "(data register .l) => A4 = 0x%x", A(4));

    // test address register .w 
    instruction = 0xd8ce;

    PC = 0x500;
    A(4) = 0x56; 
    A(6) = 0x1234;
    adda(instruction);
    
    cr_expect(A(4) == 0x128A, "Error on the destination address for adda: "
        "(address register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x56; 
    A(6) = 0x12348989;
    adda(instruction);
    
    cr_expect(A(4) == 0x89DF, "Error on the destination address for adda: "
        "(address register .w) => A4 = 0x%x", A(4));

    // test address register .l
    instruction = 0xd9ce;

    PC = 0x500;
    A(4) = 0x56; 
    A(6) = 0x1234;
    adda(instruction);
    
    cr_expect(A(4) == 0x128A, "Error on the destination address for adda: "
        "(address register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x56; 
    A(6) = 0x12348989;
    adda(instruction);
    
    cr_expect(A(4) == 0x123489DF, "Error on the destination address for adda: "
        "(address register .w) => A4 = 0x%x", A(4));
    
     // test data address register .w 
    instruction = 0xd8d1;

    PC = 0x500;
    A(4) = 0x2839; 
    A(1) = 0x7394;

    write_16bit(memory + 0x7394, 0x0984);
    adda(instruction);
    
    cr_expect(A(4) == 0x31bd, "Error on the destination address for adda: "
        "(data address register .w) => A4 = 0x%x", A(4));

    // test data address register .l
    instruction = 0xd9d1;

    PC = 0x500;
    A(4) = 0x2839; 
    A(1) = 0x7394;

    write_32bit(memory + 0x7394, 0x78780984);
    
    adda(instruction);
    
    cr_expect(A(4) == 0x787831bd, "Error on the destination address for adda: "
        "(data address register .l) => A4 = 0x%x", A(4));

    // test post data address register .w 
    instruction = 0xd8d9;

    A(4) = 0x2839; 
    A(1) = 0x7394;

    write_16bit(memory + 0x7394, 0x0984);
    adda(instruction);
    
    cr_expect(A(1) == 0x7396, "Error on the source address for adda: "
        "(post data address register .w) => A1 = 0x%x", A(1));
     cr_expect(A(4) == 0x31bd, "Error on the destination address for adda: "
        "(post data address register .w) => A4 = 0x%x", A(4));


    // test post data address register .l
    instruction = 0xd9d9;

    A(4) = 0x2839; 
    A(1) = 0x7394;

    write_32bit(memory + 0x7394, 0x78780984);
    adda(instruction);
    
    cr_expect(A(1) == 0x7398, "Error on the source address for adda: "
        "(post data address register .l) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x787831bd, "Error on the destination address for adda: "
        "(post data address register .l) => A4 = 0x%x", A(4));


    // test pre data address register .w 
    instruction = 0xd8e1;

    A(4) = 0x2839; 
    A(1) = 0x7396;

    write_16bit(memory + 0x7394, 0x0984);
    adda(instruction);
    
    cr_expect(A(1) == 0x7394, "Error on the source address for adda: "
        "(pre data address register .w) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x31bd, "Error on the destination address for adda: "
        "(pre data address register .w) => A4 = 0x%x", A(4));

    // test pre data address register .l
    instruction = 0xd9e1;

    A(4) = 0x2839; 
    A(1) = 0x7398;

    write_32bit(memory + 0x7394, 0x78780984);
    adda(instruction);
    
    cr_expect(A(1) == 0x7394, "Error on the source address for adda: "
        "(pre data address register .l) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x787831bd, "Error on the destination address for adda: "
       "(pre data address register .l) => A4 = 0x%x", A(4));

    // test mod data address register .w 
    instruction = 0xd8e9;
    
    PC = 0x500;
    A(4) = 0x2839; 
    A(1) = 0x7396;

    write_16bit(memory + PC + 2, 0xfffe);
    write_16bit(memory + 0x7394, 0x0984);
    adda(instruction);
    
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(A(1) == 0x7396, "Error on the source address for adda: "
        "(mod data address register .w) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x31bd, "Error on the destination address for adda: "
        "(mod data address register .w) => A4 = 0x%x", A(4));

    // test mod data address register .l
    instruction = 0xd9e9;
    
    PC = 0x500;
    A(4) = 0x2839; 
    A(1) = 0x7398;

    write_16bit(memory + PC + 2, 0xfffc);
    write_32bit(memory + 0x7394, 0x78780984);
    adda(instruction);
    
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(A(1) == 0x7398, "Error on the source address for adda: "
        "(mod data address register .l) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x787831bd, "Error on the destination address for adda: "
        "(mod data address register .l) => A4 = 0x%x", A(4));

    // test dir data address register .w 
    instruction = 0xd8fc;
    
    PC = 0x500;
    A(4) = 0xabab; 

    write_16bit(memory + PC + 2, 0x123);
    adda(instruction);
    
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0xacce, "Error on the destination address for adda: "
        "(dir data address register .w) => A4 = 0x%x", A(4));

    // test dir data address register .l
    instruction = 0xd9fc;
    
    PC = 0x500;
    A(4) = 0xabab; 

    write_32bit(memory + PC + 2, 0x987234);
    adda(instruction);
    
    cr_expect(PC == 0x506, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0x991DDF, "Error on the destination address for adda: "
        "(dir data address register .l) => A4 = 0x%x", A(4));
}

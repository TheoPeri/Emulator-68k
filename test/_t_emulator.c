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
    uint16_t instruction = 0x6000;
    
    // test word 
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0x00f0);
    bra(instruction);

    cr_expect(PC == 0x600, "Expect the valid new address (word >) => %x", PC);

    // test word neg
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0xfff0);
    bra(instruction);

    cr_expect(PC == 0x500, "Expect the valid new address (word <) => %x", PC);

    // test byte
    PC = 0x50e;
    instruction = 0x6030;
    
    bra(instruction);

    cr_expect(PC == 0x540, "Expect the valid new address (byte >) => %x", PC);

    // test byte neg
    PC = 0x50e;
    instruction = 0x60f0;
    
    bra(instruction);

    cr_expect(PC == 0x500, "Expect the valid new address (byte >) => %x", PC);
}

Test(emulator, test_bsr, .init=setup_emulator) {
    uint16_t instruction = 0x6100;
    
    // test word
    A(7) = 0x4;
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0x00f0);
    bsr(instruction);

    cr_expect(A(7) == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x600, "Expect the valid new address => %x", PC);
    cr_expect(0x512 == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));


    // test word neg
    A(7) = 0x4;
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0xfff0);
    bsr(instruction);

    cr_expect(A(7) == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x500, "Expect the valid new address => %x", PC);
    cr_expect(0x512 == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));

    // test byte
    A(7) = 0x4;
    PC = 0x50e;
    instruction = 0x6130;
    
    bsr(instruction);

    cr_expect(A(7) == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x540, "Expect the valid new address => %x", PC);
    cr_expect(0x510 == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));

    // test byte neg
    A(7) = 0x4;
    PC = 0x50e;
    instruction = 0x61f0;
    
    bsr(instruction);

    cr_expect(A(7) == 0x0, "Expect a push on the stack.");
    cr_expect(PC == 0x500, "Expect the valid new address => %x", PC);
    cr_expect(0x510 == read_32bit(memory), "Expect the valid return address => %x",
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


Test(emulator, test_bcc, .init=setup_emulator) {
    uint32_t instruction;

    // bhi
    PC = 0x50e;
    ZERO = 1;
    instruction = 0x6200;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bhi.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6200;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bhi.w jump <) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6200;
    write_16bit(memory + PC + 2, 0x00f0);
    bcc(instruction);
    cr_expect(PC == 0x600, "Error on the (bhi.w jump >) PC => %x", PC);

    PC = 0x50e;
    ZERO = 1;
    instruction = 0x62f0;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x510, "Error on the (bhi.b jump <) PC => %x", PC);


    PC = 0x50e;
    ZERO = 0;
    instruction = 0x62f0;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bhi.b jump <) PC => %x", PC);

    // bls
    PC = 0x50e;
    instruction = 0x6300;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bls.w no jump) PC => %x", PC);

    PC = 0x50e;
    CARRY = 1;
    NEGATIVE = 1;
    instruction = 0x6300;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bls.w jump <) PC => %x", PC);

    // bcc
    PC = 0x50e;
    CARRY = 1;
    NEGATIVE = 0;
    instruction = 0x6400;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bcc.w no jump) PC => %x", PC);

    PC = 0x50e;
    CARRY = 0;
    NEGATIVE = 1;
    instruction = 0x6400;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bcc.w jump <) PC => %x", PC);

    // bcs
    PC = 0x50e;
    CARRY = 0;
    instruction = 0x6500;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bcs.w no jump) PC => %x", PC);

    PC = 0x50e;
    CARRY = 1;
    instruction = 0x6500;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bcs.w jump <) PC => %x", PC);

    // bne
    PC = 0x50e;
    ZERO = 1;
    instruction = 0x6600;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bne.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6600;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bne.w jump <) PC => %x", PC);

    // beq
    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6700;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (beq.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 1;
    instruction = 0x6700;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (beq.w jump <) PC => %x", PC);

    // bvc
    PC = 0x50e;
    OVERFLOW = 1;
    instruction = 0x6800;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bvc.w no jump) PC => %x", PC);

    PC = 0x50e;
    OVERFLOW = 0;
    instruction = 0x6800;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bvc.w jump <) PC => %x", PC);

    // bvs
    PC = 0x50e;
    OVERFLOW = 0;
    instruction = 0x6900;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bvs.w no jump) PC => %x", PC);

    PC = 0x50e;
    OVERFLOW = 1;
    instruction = 0x6900;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bvs.w jump <) PC => %x", PC);

    // bpl
    PC = 0x50e;
    NEGATIVE = 1;
    instruction = 0x6a00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bpl.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 0;
    instruction = 0x6a00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bpl.w jump <) PC => %x", PC);

    // bmi
    PC = 0x50e;
    NEGATIVE = 0;
    instruction = 0x6b00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bmi.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 1;
    instruction = 0x6b00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bmi.w jump <) PC => %x", PC);

    // bge
    PC = 0x50e;
    NEGATIVE = 1;
    OVERFLOW = 0;
    instruction = 0x6c00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bge.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6c00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bge.w jump <) PC => %x", PC);

    // blt
    PC = 0x50e;
    NEGATIVE = 1;
    OVERFLOW = 1;
    instruction = 0x6d00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (blt.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 0;
    OVERFLOW = 1;
    instruction = 0x6d00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (blt.w jump <) PC => %x", PC);

    // bgt
    PC = 0x50e;
    ZERO = 1;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6e00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (bgt.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6e00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (bgt.w jump <) PC => %x", PC);

    // ble
    PC = 0x50e;
    ZERO = 1;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6f00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x512, "Error on the (ble.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6f00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_expect(PC == 0x500, "Error on the (ble.w jump <) PC => %x", PC);

}

Test(emulator, test_cmp, .init=setup_emulator) {
    uint32_t instruction;

    // test data register .b
    instruction = 0xb200;

    PC = 0x50c;
    D(0) = 0x2; 
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(PC == 0x50e, "Error on the PC => %x", PC);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    D(1) = 0x2;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    D(1) = -0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    D(1) = 1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 1; 
    D(1) = -128;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -1; 
    D(1) = -128;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    D(1) = -1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data register .w
    instruction = 0xb240;
    PC = 0x50c;
    D(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data register .l 
    instruction = 0xb280;
    PC = 0x50c;
    D(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test address register .w 
    instruction = 0xb248;
    PC = 0x50c;
    A(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test address register .l
    instruction = 0xb288;
    PC = 0x50c;
    A(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data address register .b
    instruction = 0xb210;
    PC = 0x50c;
    A(0) = 0x7000; 
    write_8bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data address register .w
    instruction = 0xb250;
    PC = 0x50c;
    A(0) = 0x7000; 
    write_16bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data address register .l
    instruction = 0xb290;
    PC = 0x50c;
    A(0) = 0x7000; 
    write_32bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    
    // test post data address register .b
    instruction = 0xb218;
    PC = 0x50c;
    A(0) = 0x7000; 
    write_8bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    cr_expect(A(0) == 0x7001, "Error on the address after post incrementation"
        " => %x", A(0));

    // test post data address register .w
    instruction = 0xb258;
    PC = 0x50c;
    A(0) = 0x7000; 
    write_16bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    cr_expect(A(0) == 0x7002, "Error on the address after post incrementation"
        " => %x", A(0));

    // test post data address register .l
    instruction = 0xb298;
    PC = 0x50c;
    A(0) = 0x7000; 
    write_32bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    cr_expect(A(0) == 0x7004, "Error on the address after post incrementation"
        " => %x", A(0));
    
    // test pre data address register .b
    instruction = 0xb220;
    PC = 0x50c;
    A(0) = 0x7001; 
    write_8bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    cr_expect(A(0) == 0x7000, "Error on the address after post incrementation"
        " => %x", A(0));

    // test pre data address register .w
    instruction = 0xb260;
    PC = 0x50c;
    A(0) = 0x7002; 
    write_16bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    cr_expect(A(0) == 0x7000, "Error on the address after post incrementation"
        " => %x", A(0));

    // test pre data address register .l
    instruction = 0xb2a0;
    PC = 0x50c;
    A(0) = 0x7004; 
    write_32bit(memory + 0x7000, -1);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    cr_expect(A(0) == 0x7000, "Error on the address after post incrementation"
        " => %x", A(0));

    // test mod data address register .b
    instruction = 0xb228;
    PC = 0x50c;
    A(0) = 0x8000; 
    write_8bit(memory + 0x7000, -1);
    write_16bit(memory + PC + 2, 0xF000);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    cr_expect(A(0) == 0x8000, "Error on the address after post incrementation"
        " => %x", A(0));

    // test dir data address register .b
    instruction = 0x0c01;
    PC = 0x50c;
    write_16bit(memory + PC + 2, 0x00ff);
    D(1) = 0x1;
    cmp(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
}

Test(emulator, test_cmpa, .init=setup_emulator) {
    uint32_t instruction;

    // test data register .w
    instruction = 0xb2c0;

    PC = 0x50c;
    D(0) = 0x2; 
    A(1) = 0x1;
    cmpa(instruction);
    cr_expect(PC == 0x50e, "Error on the PC => %x", PC);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    A(1) = 0x2;
    cmpa(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    A(1) = 0x1;
    cmpa(instruction);
    cr_expect(ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -0x1; 
    A(1) = 0x1;
    cmpa(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    A(1) = -0x1;
    cmpa(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    A(1) = 1;
    cmpa(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 1; 
    A(1) = -128;
    cmpa(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -1; 
    A(1) = -128;
    cmpa(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    A(1) = -1;
    cmpa(instruction);
    cr_expect(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);
}


Test(emulator, test_cmpm, .init=setup_emulator) {
    uint32_t instruction;

    instruction = 0xb308;

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 2; 
    A(1) = 0x8000;
    memory[0x8000] = 1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = 2; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = 1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -1; 
    A(1) = 0x8000;
    memory[0x8000] = 1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = 1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = -128; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -1; 
    A(1) = 0x8000;
    memory[0x8000] = -128; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_expect(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_expect(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_expect(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_expect(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test incrementation .w
    instruction = 0xb348;
    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7002, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8002, "Error on the address A1 => %x", A(1));
  
    // test incrementation .l
    instruction = 0xb388;
    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_expect(PC == 0x516, "Error on the PC => %x", PC);
    cr_expect(A(0) == 0x7004, "Error on the address A0 => %x", A(0));
    cr_expect(A(1) == 0x8004, "Error on the address A1 => %x", A(1));
}



Test(emulator, test_movea, .init=setup_emulator) {
    uint16_t instruction;

    // test data register .w 
    instruction = 0x3842;
    PC = 0x500;
    A(4) = 0x5678;
    D(2) = 0x87654321;
    movea(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0x00004321, "Error on the destination address for movea: "
        "(data register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x12345678;
    D(2) = 0x87654321;
    movea(instruction);
    cr_expect(A(4) == 0x00004321, "Error on the destination address for movea: "
        "(data register .w) => A4 = 0x%x", A(4));

    // test data register .l 
    instruction = 0x2842;

    PC = 0x500;
    A(4) = 0x12345678;
    D(2) = 0x4321;
    movea(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0x4321, "Error on the destination address for movea: "
        "(data register .l) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x12345678;
    D(2) = 0x87654321;
    movea(instruction);
    cr_expect(A(4) == 0x87654321, "Error on the destination address for movea: "
        "(data register .l) => A4 = 0x%x", A(4));

    // test address register .w 
    instruction = 0x384e;

    PC = 0x500;
    A(4) = 0x12345678; 
    A(6) = 0x4321;
    movea(instruction);
    
    cr_expect(A(4) == 0x4321, "Error on the destination address for movea: "
        "(address register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x66666665; 
    A(6) = 0x12121212;
    movea(instruction);
    
    cr_expect(A(4) == 0x1212, "Error on the destination address for movea: "
        "(address register .w) => A4 = 0x%x", A(4));

    // test address register .l
    instruction = 0x284e;

    PC = 0x500;
    A(4) = 0x12346666;
    A(6) = 0x1234;
    movea(instruction);
    
    cr_expect(A(4) == 0x1234, "Error on the destination address for movea: "
        "(address register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x12345555; 
    A(6) = 0x12348989;
    movea(instruction);
    
    cr_expect(A(4) == 0x12348989, "Error on the destination address for movea: "
        "(address register .w) => A4 = 0x%x", A(4));

     // test data address register .w 
    instruction = 0x38d1;

    PC = 0x500;
    A(4) = 0x2839;
    A(1) = 0x7394;

    write_16bit(memory + 0x7394, 0x0984);
    movea(instruction);
    
    cr_expect(A(4) == 0x0984, "Error on the destination address for movea: "
        "(data address register .w) => A4 = 0x%x", A(4));

    // test data address register .l
    instruction = 0x28d1;

    PC = 0x500;
    A(4) = 0x2839;
    A(1) = 0x7394;

    write_32bit(memory + 0x7394, 0x78780984);
    
    movea(instruction);
    
    cr_expect(A(4) == 0x78780984, "Error on the destination address for movea: "
        "(data address register .l) => A4 = 0x%x", A(4));

    // test post data address register .w 
    instruction = 0x3859;

    A(4) = 0x2839;
    A(1) = 0x7394;

    write_16bit(memory + 0x7394, 0x0984);
    movea(instruction);
    
    cr_expect(A(1) == 0x7396, "Error on the source address for movea: "
        "(post data address register .w) => A1 = 0x%x", A(1));
     cr_expect(A(4) == 0x0984, "Error on the destination address for movea: "
        "(post data address register .w) => A4 = 0x%x", A(4));


    // test post data address register .l
    instruction = 0x2859;

    A(4) = 0x2839; 
    A(1) = 0x7394;

    write_32bit(memory + 0x7394, 0x78780984);
    movea(instruction);
    
    cr_expect(A(1) == 0x7398, "Error on the source address for movea: "
        "(post data address register .l) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x78780984, "Error on the destination address for movea: "
        "(post data address register .l) => A4 = 0x%x", A(4));


    // test pre data address register .w 
    instruction = 0x3861;

    A(4) = 0x2839; 
    A(1) = 0x7396;

    write_16bit(memory + 0x7394, 0x0984);
    movea(instruction);
    
    cr_expect(A(1) == 0x7394, "Error on the source address for movea: "
        "(pre data address register .w) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x0984, "Error on the destination address for movea: "
        "(pre data address register .w) => A4 = 0x%x", A(4));

    // test pre data address register .l
    instruction = 0x2861;

    A(4) = 0x2839; 
    A(1) = 0x7398;

    write_32bit(memory + 0x7394, 0x78780984);
    movea(instruction);
    
    cr_expect(A(1) == 0x7394, "Error on the source address for movea: "
        "(pre data address register .l) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x78780984, "Error on the destination address for movea: "
       "(pre data address register .l) => A4 = 0x%x", A(4));

    // test mod data address register .w 
    instruction = 0x3869;
    
    PC = 0x500;
    A(4) = 0x2839; 
    A(1) = 0x7396;

    write_16bit(memory + PC + 2, 0xfffe);
    write_16bit(memory + 0x7394, 0x0984);
    movea(instruction);
    
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(A(1) == 0x7396, "Error on the source address for movea: "
        "(mod data address register .w) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x0984, "Error on the destination address for movea: "
        "(mod data address register .w) => A4 = 0x%x", A(4));

    // test mod data address register .l
    instruction = 0x2869;
    
    PC = 0x500;
    A(4) = 0x2839; 
    A(1) = 0x7398;

    write_16bit(memory + PC + 2, 0xfffc);
    write_32bit(memory + 0x7394, 0x78780984);
    movea(instruction);
    
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(A(1) == 0x7398, "Error on the source address for movea: "
        "(mod data address register .l) => A1 = 0x%x", A(1));
    cr_expect(A(4) == 0x78780984, "Error on the destination address for movea: "
        "(mod data address register .l) => A4 = 0x%x", A(4));

    // test dir data address register .w 
    instruction = 0x387c;
    
    PC = 0x500;
    A(4) = 0xabab; 

    write_16bit(memory + PC + 2, 0x123);
    movea(instruction);
    
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0x123, "Error on the destination address for movea: "
        "(dir data address register .w) => A4 = 0x%x", A(4));

    // test dir data address register .l
    instruction = 0x287c;
    
    PC = 0x500;
    A(4) = 0xabab; 

    write_32bit(memory + PC + 2, 0x987234);
    movea(instruction);
    
    cr_expect(PC == 0x506, "Error on the PC => %x", PC);
    cr_expect(A(4) == 0x987234, "Error on the destination address for movea: "
        "(dir data address register .l) => A4 = 0x%x", A(4));

}

Test(emulator, test_move, .init=setup_emulator) {
    uint16_t instruction;
    
    // test data register .b
    instruction = 0x1202;

    //move.b		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    D(2) = 0x87654321;
    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x12345621, "Error on the destination address for move: "
        "(data register .b) => D1 = 0x%x", D(1));


   // test data register .w
    instruction = 0x3202;

    //move.w		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    D(2) = 0x87654321;
    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x12344321, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x2202;

    //move.l		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    D(2) = 0x87654321;
    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x87654321, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));



   // test data register .w
    instruction = 0x320A;

    //move.w		A2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x87654321;
    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x12344321, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x220A;

    //move.l		A2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x87654321;
    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x87654321, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));


   // test data register .b
    instruction = 0x1212;

    //move.b		(A2),D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_8bit(memory + 0x7394, 0x84);

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x12345684, "Error on the destination address for move: "
    "(data register .b) => D1 = 0x%x", D(1));



   // test data register .w
    instruction = 0x3212;

    //move.w		(A2),D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_16bit(memory + 0x7394, 0x0984);

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x12340984, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x2212;

    //move.l		(A2),D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_32bit(memory + 0x7394, 0x78780984);

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x78780984, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));


   // test data register .b
    instruction = 0x121A;

    //move.b		(A2)+,D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_8bit(memory + 0x7394, 0x84);

    move(instruction);
    cr_expect(A(2) == 0x7395, "Error on the source address for move: "
        "(post data address register .b) => A2 = 0x%x", A(2));
   cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x12345684, "Error on the destination address for move: "
    "(data register .b) => D1 = 0x%x", D(1));



   // test data register .w
    instruction = 0x321A;

    //move.w		(A2)+,D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_16bit(memory + 0x7394, 0x0984);

    move(instruction);
    cr_expect(A(2) == 0x7396, "Error on the source address for move: "
        "(post data address register .w) => A2 = 0x%x", A(2));
 cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x12340984, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x221A;

    //move.l		(A2)+,D1

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_32bit(memory + 0x7394, 0x78780984);

    move(instruction);
    cr_expect(A(2) == 0x7398, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x78780984, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));

///   its the same than movea

   // test data register .b
    instruction = 0x1481;

    //move.b		D1,(A2)

    PC = 0x500;
    D(1) = 0x65;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)] == 0x65, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)]);



   // test data register .w
    instruction = 0x3481;

    //move.w		D1,(A2)

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)) == 0x5678, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)));


    // test data register .l 
    instruction = 0x2481;

    //move.l		D1,(A2)

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)) == 0x12345678, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)));



   // test data register .b
    instruction = 0x14c1;

    //move.b		D1,(A2)+

    PC = 0x500;
    D(1) = 0x65656565;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(A(2) == 0x7395, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)-1] == 0x65, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)-1]);



   // test data register .w
    instruction = 0x34c1;

    //move.w		D1,(A2)+

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(A(2) == 0x7396, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)-2) == 0x5678, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)-2));


    // test data register .l 
    instruction = 0x24c1;

    //move.l		D1,(A2)+

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(A(2) == 0x7398, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)-4) == 0x12345678, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)-4));



   // test data register .b
    instruction = 0x1501;

    //move.b		D1,-(A2)

    PC = 0x500;
    D(1) = 0x65656565;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(A(2) == 0x7393, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)] == 0x65, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)]);



   // test data register .w
    instruction = 0x3501;

    //move.w		D1,-(A2)

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(A(2) == 0x7392, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)) == 0x5678, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)));


    // test data register .l 
    instruction = 0x2501;

    //move.l		D1,-(A2)

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(A(2) == 0x7390, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)) == 0x12345678, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)));



   // test data register .b
    instruction = 0x1541;

    //move.b		D1,5(A2)

    PC = 0x500;
    D(1) = 0x65656565;
    A(2) = 0x7394;
    write_16bit(memory + PC + 2, 0x5);
    
    move(instruction);
    cr_expect(A(2) == 0x7394, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)+5] == 0x65, "Error on the destination address for move: "
    "(data register .b) => 5(A2) = 0x%x", memory[A(2)+5]);



   // test data register .w
    instruction = 0x3541;

    //move.w		D1,5(A2)

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_16bit(memory + PC + 2, 0x5);

    move(instruction);
    cr_expect(A(2) == 0x7394, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)+5) == 0x5678, "Error on the destination address for move: "
    "(data register .w) => 5(A2) = 0x%x", read_16bit(memory + A(2)+5));


    // test data register .l 
    instruction = 0x2541;

    //move.l		D1,5(A2)

    PC = 0x500;
    D(1) = 0x12345678;
    A(2) = 0x7394;
    write_16bit(memory + PC + 2, 0x5);

    move(instruction);
    cr_expect(A(2) == 0x7394, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x504, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)+5) == 0x12345678, "Error on the destination address for move: "
        "(data register .l) => 5(A2) = 0x%x", read_32bit(memory + A(2)+5));


   // test data register .b
    instruction = 0x1569;

    //move.b		3(A1),5(A2)

    PC = 0x500;
    A(1) = 0x1234;
    A(2) = 0x6394;
    write_16bit(memory + PC + 2, 0x3);
    write_16bit(memory + PC + 4, 0x5);
    
    memory[A(1)+3] = 0x69;

    move(instruction);
    cr_expect(A(2) == 0x6394, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x506, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)+5] == 0x69, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)+5]);



   // test data register .w
    instruction = 0x3569;

    //move.w		3(A1),5(A2)

    PC = 0x500;
    A(1) = 0x1534;
    A(2) = 0x6394;
    write_16bit(memory + PC + 2, 0x3);
    write_16bit(memory + PC + 4, 0x5);
    
    write_16bit(memory + A(1) +3, 0x1233);


    move(instruction);
    cr_expect(A(2) == 0x6394, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x506, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)+5) == 0x1233, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)+5));


    // test data register .l 
    instruction = 0x2569;

    //move.l		3(A1),5(A2)

    PC = 0x500;
    A(1) = 0x5678;
    A(2) = 0x6394;
    write_16bit(memory + PC + 2, 0x3);
    write_16bit(memory + PC + 4, 0x5);
    
    write_32bit(memory + A(1) +3, 0x84268426);


    move(instruction);
    cr_expect(A(2) == 0x6394, "Error on the source address for move: "
        "(post data address register .l) => A2 = 0x%x", A(2));
    cr_expect(PC == 0x506, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)+5) == 0x84268426, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)+5));


   // test data register .b
    instruction = 0x1481;

    //move.b		D1,(A2)

    PC = 0x500;
    D(1) = 0x0;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)] == 0x0, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)]);

    cr_expect(ZERO == 0x1, "Error on the ZERO => %x", ZERO);


   // test data register .w
    instruction = 0x3481;

    //move.w		D1,(A2)

    PC = 0x500;
    D(1) = 0x0;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)) == 0x0, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)));

    cr_expect(ZERO == 0x1, "Error on the ZERO => %x", ZERO);

    // test data register .l 
    instruction = 0x2481;

    //move.l		D1,(A2)

    PC = 0x500;
    D(1) = 0x0;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)) == 0x0, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)));


    cr_expect(ZERO == 0x1, "Error on the ZERO => %x", ZERO);

   // test data register .b
    instruction = 0x1481;



    //move.b		D1,(A2)

    PC = 0x500;
    D(1) = 0x66;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)] == 0x66, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)]);

    cr_expect(ZERO == 0x0, "Error on the ZERO => %x", ZERO);


   // test data register .w
    instruction = 0x3481;

    //move.w		D1,(A2)

    PC = 0x500;
    D(1) = 0x5656;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)) == 0x5656, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)));

    cr_expect(ZERO == 0x0, "Error on the ZERO => %x", ZERO);

    // test data register .l 
    instruction = 0x2481;

    //move.l		D1,(A2)

    PC = 0x500;
    D(1) = 0x159159;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)) == 0x159159, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)));


    cr_expect(ZERO == 0x0, "Error on the ZERO => %x", ZERO);


   // test data register .b
    instruction = 0x1481;

    //move.b		D1,(A2)

    PC = 0x500;
    D(1) = 0xffffff80;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)] == 0x80, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)]);

    cr_expect(NEGATIVE == 0x1, "Error on the NEGATIVE => %x", NEGATIVE);


   // test data register .w
    instruction = 0x3481;

    //move.w		D1,(A2)

    PC = 0x500;
    D(1) = 0xffff8000;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)) == 0x8000, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)));

    cr_expect(NEGATIVE == 0x1, "Error on the NEGATIVE => %x", NEGATIVE);

    // test data register .l 
    instruction = 0x2481;

    //move.l		D1,(A2)

    PC = 0x500;
    D(1) = 0x80000000;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)) == 0x80000000, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)));


    cr_expect(NEGATIVE == 0x1, "Error on the NEGATIVE => %x", NEGATIVE);


   // test data register .b
    instruction = 0x1481;

    //move.b		D1,(A2)

    PC = 0x500;
    D(1) = 0xffffff7f;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(memory[A(2)] == 0x7f, "Error on the destination address for move: "
    "(data register .b) => (A2) = 0x%x", memory[A(2)]);

    cr_expect(NEGATIVE == 0x0, "Error on the NEGATIVE => %x", NEGATIVE);


   // test data register .w
    instruction = 0x3481;

    //move.w		D1,(A2)

    PC = 0x500;
    D(1) = 0xffff7fff;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_16bit(memory + A(2)) == 0x7fff, "Error on the destination address for move: "
    "(data register .w) => (A2) = 0x%x", read_16bit(memory + A(2)));

    cr_expect(NEGATIVE == 0x0, "Error on the NEGATIVE => %x", NEGATIVE);

    // test data register .l 
    instruction = 0x2481;

    //move.l		D1,(A2)

    PC = 0x500;
    D(1) = 0x7fffffff;
    A(2) = 0x7394;

    move(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(read_32bit(memory + A(2)) == 0x7fffffff, "Error on the destination address for move: "
        "(data register .l) => (A2) = 0x%x", read_32bit(memory + A(2)));


    cr_expect(NEGATIVE == 0x0, "Error on the NEGATIVE => %x", NEGATIVE);
}




Test(emulator, test_moveq, .init=setup_emulator) {
    uint16_t instruction;
    
    // test data register .b
    instruction = 0x7208;

    //move.b		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x8, "Error on the destination address for move: "
        "(data register .b) => D1 = 0x%x", D(1));


   // test data register .w
    instruction = 0x7240;

    //move.w		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x40, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x727f;

    //move.l		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0x7f, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));

 
    // test data register .b
    instruction = 0x7C08;

    //move.b		D2,D1

    PC = 0x500;
    D(6) = 0x12345678;
    moveq(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(6) == 0x8, "Error on the destination address for move: "
        "(data register .b) => D6 = 0x%x", D(6));


   
    // test data register .b
    instruction = 0x72f8;

    //move.b		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0xfffffff8, "Error on the destination address for move: "
        "(data register .b) => D1 = 0x%x", D(1));


   // test data register .w
    instruction = 0x72c0;

    //move.w		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0xffffffc0, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x7281;

    //move.l		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_expect(PC == 0x502, "Error on the PC => %x", PC);
    cr_expect(D(1) == 0xffffff81, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));

}




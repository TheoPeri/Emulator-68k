#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "../src/memory.h"
#include "../src/emulator.h"

void setup_emulator(void) {
    memory = calloc(16777220, 1);
}

Test(emulator, test_addressing_mode_source, .init=setup_emulator) {
    uint8_t value;
    uint32_t source;
    uint32_t displacement = 0;


    // data register (no cast)
    value = 0x02; // d2 
    D(2) = 0x09872345;

    source = addressing_mode_source(0, value, &displacement);
    cr_assert(source == 0x45, "| data register (byte) | Expect source == D2(0x%x)", D(2));
    cr_assert(!displacement, "| data register (byte) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    value = 0x02; // d2 
    D(2) = 0x09872345;

    source = addressing_mode_source(1, value, &displacement);
    cr_assert(source == 0x2345, "| data register (word) | Expect source == D2(0x%x)", D(2));
    cr_assert(!displacement, "| data register (word) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    value = 0x02; // d2 
    D(2) = 0x09872345;

    source = addressing_mode_source(2, value, &displacement);
    cr_assert(source == 0x09872345, "| data register (long) | Expect source == D2(0x%x)", D(2));
    cr_assert(!displacement, "| data register (long) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op



    // address register (no cast)
    value = 0x0f; // a7
    A(7) = 0x003223233;

    source = addressing_mode_source(0, value, &displacement);
    cr_assert(source == 0x003223233, "| address register | Expect source == A7(0x%x)", A(7));
    cr_assert(!displacement, "| address register | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    
    // address
    // the size is byte
    value = 0x14; // a4
    A(4) = 0x03489; // address of the value
    memory[A(4)] = 0x37; // the expected source value
    
    source = addressing_mode_source(0, value, &displacement);
    cr_assert(source == 0x37, "| address (byte) | Expect source == (A4)(0x%x)", memory[A(4)]);
    cr_assert(!displacement, "| address (byte) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    // the size is word
    value = 0x12; // a2
    A(2) = 0x1023; // address of the value
    write_16bit(memory + A(2), 0x9823); // the expected source value
    
    source = addressing_mode_source(1, value, &displacement);
    cr_assert(source == 0x9823, "| address (word) | Expect source == "
        "(A2)(0x%x)", read_16bit(memory + A(2)));
    cr_assert(!displacement, "| address (word) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    // the size is long
    value = 0x11; // a1
    A(1) = 0x101923; // address of the value
    write_32bit(memory + A(1), 0x12089823); // the expected source value
    
    source = addressing_mode_source(2, value, &displacement);
    cr_assert(source == 0x12089823, "| address (long) | Expect source == "
        "(A1)(0x%x)", read_32bit(memory + A(1)));
    cr_assert(!displacement, "| address (long) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op


    // address postincrement
    // the size is byte
    value = 0x1e; // a6
    A(6) = 0x03028; // address of the value
    memory[A(6)] = 0x90; // the expected source value
    
    source = addressing_mode_source(0, value, &displacement);
    cr_assert(source == 0x90, "| address postincrement (byte) | Expect source "
        "== (A6)+(0x%x)", memory[0x03028]);
    cr_assert(!displacement, "| address postincrement (byte) | Expect "
        "displacement to be  not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(6) == 0x03029, "| address postincrement (byte) | Expect a "
        " postincrementation"); // check postincrementation (+1)

    // the size is word
    value = 0x18; // a0
    A(0) = 0x23908; 
    write_16bit(memory + A(0), 0x1689); // the expected source value
    
    source = addressing_mode_source(1, value, &displacement);
    cr_assert(source == 0x1689, "| address postincrement (word) | Expect "
        "source == (A0)+(0x%x)", read_16bit(memory + A(0) - 2));
    cr_assert(!displacement, "| address postincrement (word) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(0) == 0x2390a, "| address postincrement (word) | Expect a "
        " postincrementation"); // check postincrementation (+2)

    // the size is long
    value = 0x1a; // a2
    A(2) = 0x29487;
    write_32bit(memory + A(2), 0x12828933); // the expected source value
    
    source = addressing_mode_source(2, value, &displacement);
    cr_assert(source == 0x12828933, "| address postincrement (long) | Expect "
        "source == (A2)+(0x%x)", read_32bit(memory + A(2) - 4));
    cr_assert(!displacement, "| address postincrement (long) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(2) == 0x2948b, "| address postincrement (long) | Expect a "
        " postincrementation"); // check postincrementation (+4)

    
    // address predecrement
    // the size is byte
    value = 0x26; // a6
    A(6) = 0x03029; // address of the value
    memory[0x03028] = 0x90; // the expected source value
    
    source = addressing_mode_source(0, value, &displacement);
    cr_assert(source == 0x90, "| address predecrement (byte) | Expect source "
        "== (A6)+(0x%x)", memory[0x03028]);
    cr_assert(!displacement, "| address predecrement (byte) | Expect "
        "displacement to be  not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(6) == 0x03028, "| address predecrement (byte) | Expect a "
        " postincrementation"); // check postincrementation (-1)

    // the size is word
    value = 0x20; // a0
    A(0) = 0x2390a; 
    write_16bit(memory + 0x23908, 0x1689); // the expected source value
    
    source = addressing_mode_source(1, value, &displacement);
    cr_assert(source == 0x1689, "| address predecrement (word) | Expect "
        "source == (A0)+(0x%x)", read_16bit(memory + A(0)));
    cr_assert(!displacement, "| address predecrement (word) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(0) == 0x23908, "| address predecrement (word) | Expect a "
        " postincrementation"); // check postincrementation (-2)

    // the size is long
    value = 0x22; // a2
    A(2) = 0x2948b;
    write_32bit(memory + 0x29487, 0x12828933); // the expected source value
    
    source = addressing_mode_source(2, value, &displacement);
    cr_assert(source == 0x12828933, "| address predecrement (long) | Expect "
        "source == (A2)+(0x%x)", read_32bit(memory + A(2)));
    cr_assert(!displacement, "| address predecrement (long) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(2) == 0x29487, "| address predecrement (long) | Expect a "
        " postincrementation"); // check postincrementation (-4)


    // address displacement
    PC = 0x8998;
    // the size is byte
    value = 0x2e; // a6
    A(6) = 0x03029; // address of the value
    write_16bit(memory + PC + 2, 0x1689); // 0x1689
    memory[0x03029 + 0x1689] = 0x90; // the expected source value
    
    source = addressing_mode_source(0, value, &displacement);
    cr_assert(source == 0x90, "| address displacement (byte) | Expect source "
        "== 0x1689(A6)(0x%x)", memory[0x03029 + 0x1689]);
    cr_assert(displacement == 2, "| address displacement (byte) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op

    // the size is word
    value = 0x28; // a0
    displacement = 0;
    A(0) = 0x22390;
    write_16bit(memory + PC + 2, 0x1689); // 0x1689
    write_16bit(memory + A(0) + 0x1689, 0x1689); // the expected source value
    
    source = addressing_mode_source(1, value, &displacement);
    cr_assert(source == 0x1689, "| address displacement (word) | Expect "
        "source == 0x1689(A0)(0x%x)", read_16bit(memory + A(0) + 0x1689));
    cr_assert(displacement == 2, "| address displacement (word) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op

    // the size is long
    value = 0x2a; // a2
    displacement = 0;
    A(2) = 0x2948b;
    write_16bit(memory + PC + 2, 0xfff4); // -12
    write_32bit(memory + 0x2948b - 12, 0x12828933); // the expected source value
    
    source = addressing_mode_source(2, value, &displacement);
    cr_assert(source == 0x12828933, "| address displacement (long) | Expect "
        "source == -12(A2)(0x%x)", read_32bit(memory + A(2) - 12));
    cr_assert(displacement == 2, "| address displacement (long) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op


    // immediate
    PC = 0x8998;
    // the size is byte
    displacement = 0;
    value = 0x3c; // a6
    write_16bit(memory + PC + 2, 0x67); // 0x1689
    
    source = addressing_mode_source(0, value, &displacement);
    cr_assert(source == 0x67, "| immediate (byte) | Expect source "
        "== #i(0xfff4)");
    cr_assert(displacement == 2, "| immediate (byte) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op

    // the size is word
    displacement = 0;
    value = 0x3c; // a0
    displacement = 0;
    write_16bit(memory + PC + 2, 0x1689); // 0x1689
    
    source = addressing_mode_source(1, value, &displacement);
    cr_assert(source == 0x1689, "| immediate (word) | Expect "
        "source == #i(0xfff4)");
    cr_assert(displacement == 2, "| immediate (word) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op

    // the size is long
    displacement = 0;
    value = 0x3c; // a2
    displacement = 0;
    write_32bit(memory + PC + 2, 0xfff4); // -12
    
    source = addressing_mode_source(2, value, &displacement);
    cr_assert(source == 0xfff4, "| immediate (long) | Expect "
        "source == #i(0xfff4)");
    cr_assert(displacement == 4, "| immediate (long) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
}

Test(emulator, test_addressing_mode_destination, .init=setup_emulator) {
    uint8_t value;
    uint32_t displacement = 0;


    // data register (no cast)
    // the size is byte
    value = 0x02; // d2 
    D(2) = 0x09872345;

    addressing_mode_destination(0, value, &displacement, 0x12345678);
    cr_assert(D(2) == 0x09872378, "| data register (byte) | Expect D2 == 0x09872378 (0x%x)", D(2));
    cr_assert(!displacement, "| address register | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op
    
    // the size is word
    value = 0x02; // d2 
    D(2) = 0x09872345;

    addressing_mode_destination(1, value, &displacement, 0x12345678);
    cr_assert(D(2) == 0x09875678, "| data register (word) | Expect D2 == 0x09875678 (0x%x)", D(2));
    cr_assert(!displacement, "| address register | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    // the size is long
    value = 0x02; // d2 
    D(2) = 0x09872345;

    addressing_mode_destination(2, value, &displacement, 0x12345678);
    cr_assert(D(2) == 0x12345678, "| data register (long) | Expect D2 == 0x12345678 (0x%x)", D(2));
    cr_assert(!displacement, "| address register | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op


    // address
    // the size is byte
    value = 0x14; // a4
    A(4) = 0x03489; // address of the value
    memory[A(4)] = 0x37; // the expected source value
    
    addressing_mode_destination(0, value, &displacement, 0x12345678);
    cr_assert(memory[A(4)] == 0x78, "| address (byte) | Expect"
        " (A4)(0x%x) == 0x78", memory[A(4)]);
    cr_assert(!displacement, "| address (byte) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    // the size is word
    value = 0x12; // a2
    A(2) = 0x1023; // address of the value
    write_16bit(memory + A(2), 0x9823); // the expected source value
    
    addressing_mode_destination(1, value, &displacement, 0x12345678);
    cr_assert(read_16bit(memory + A(2)) == 0x5678, "| address (word) | Expect "
        "(A2)(0x%x) == 0x5678", read_16bit(memory + A(2)));
    cr_assert(!displacement, "| address (word) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op

    // the size is long
    value = 0x11; // a1
    A(1) = 0x101923; // address of the value
    write_32bit(memory + A(1), 0x12089823); // the expected source value
    
    addressing_mode_destination(2, value, &displacement, 0x12345678);
    cr_assert(read_32bit(memory + A(1)) == 0x12345678, "| address (long) | "
        "Expect (A1)(0x%x) == 0x12345678", read_32bit(memory + A(1)));
    cr_assert(!displacement, "| address (long) | Expect displacement to be"
        " not modified (value of the modification: 0x%x)", displacement); // no displacement for this op


    // address postincrement
    // the size is byte
    value = 0x1e; // a6
    A(6) = 0x03028; // address of the value
    memory[A(6)] = 0x90; // the expected source value
    
    addressing_mode_destination(0, value, &displacement, 0x12345678);
    cr_assert(memory[A(6) - 1] == 0x78, "| address postincrement (byte) | Expect source "
        "== (A6)+(0x%x)", memory[0x03028]);
    cr_assert(!displacement, "| address postincrement (byte) | Expect "
        "displacement to be  not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(6) == 0x03029, "| address postincrement (byte) | Expect a "
        " postincrementation"); // check postincrementation (+1)

    // the size is word
    value = 0x18; // a0
    A(0) = 0x23908; 
    write_16bit(memory + A(0), 0x1689); // the expected source value
    
    addressing_mode_destination(1, value, &displacement, 0x12345678);
    cr_assert(read_16bit(memory + A(0) - 2) == 0x5678, "| address "
        "postincrement (word) | Expect source == (A0)+(0x%x)",
        read_16bit(memory + A(0) - 2));
    cr_assert(!displacement, "| address postincrement (word) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(0) == 0x2390a, "| address postincrement (word) | Expect a "
        " postincrementation"); // check postincrementation (+2)

    // the size is long
    value = 0x1a; // a2
    A(2) = 0x29487;
    write_32bit(memory + A(2), 0x12828933); // the expected source value
    
    addressing_mode_destination(2, value, &displacement, 0x12345678);
    cr_assert(read_32bit(memory + A(2) - 4) == 0x12345678, "| address "
        "postincrement (long) | Expect source == (A2)+(0x%x)",
        read_32bit(memory + A(2) - 4));
    cr_assert(!displacement, "| address postincrement (long) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(2) == 0x2948b, "| address postincrement (long) | Expect a "
        " postincrementation"); // check postincrementation (+4)

    
    // address predecrement
    // the size is byte
    value = 0x26; // a6
    A(6) = 0x03029; // address of the value
    memory[0x03028] = 0x90; // the expected source value
    
    addressing_mode_destination(0, value, &displacement, 0x12345678);
    cr_assert(memory[0x03028] == 0x78, "| address predecrement (byte) | Expect source "
        "== (A6)+(0x%x)", memory[0x03028]);
    cr_assert(!displacement, "| address predecrement (byte) | Expect "
        "displacement to be  not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(6) == 0x03028, "| address predecrement (byte) | Expect a "
        " postincrementation"); // check postincrementation (-1)

    // the size is word
    value = 0x20; // a0
    A(0) = 0x2390a; 
    write_16bit(memory + 0x23908, 0x1689); // the expected source value
    
    addressing_mode_destination(1, value, &displacement, 0x12345678);
    cr_assert(read_16bit(memory + A(0)) == 0x5678, "| address predecrement (word) | Expect "
        "source == (A0)+(0x%x)", read_16bit(memory + A(0)));
    cr_assert(!displacement, "| address predecrement (word) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(0) == 0x23908, "| address predecrement (word) | Expect a "
        " postincrementation"); // check postincrementation (-2)

    // the size is long
    value = 0x22; // a2
    A(2) = 0x2948b;
    write_32bit(memory + 0x29487, 0x12828933); // the expected source value
    
    addressing_mode_destination(2, value, &displacement, 0x12345678);
    cr_assert(read_32bit(memory + A(2)) == 0x12345678, "| address predecrement (long) | Expect "
        "source == (A2)+(0x%x)", read_32bit(memory + A(2)));
    cr_assert(!displacement, "| address predecrement (long) | Expect "
        "displacement to be not modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
    cr_assert(A(2) == 0x29487, "| address predecrement (long) | Expect a "
        " postincrementation"); // check postincrementation (-4)


    // address displacement
    PC = 0x8998;
    // the size is byte
    value = 0x2e; // a6
    A(6) = 0x03029; // address of the value
    write_16bit(memory + PC + 2, 0x1689); // 0x1689
    memory[0x03029 + 0x1689] = 0x90; // the expected source value
    
    addressing_mode_destination(0, value, &displacement, 0x12345678);
    cr_assert(memory[0x03029 + 0x1689] == 0x78, "| address displacement "
        "(byte) | Expect source == 0x1689(A6)(0x%x)", memory[0x03029 + 0x1689]);
    cr_assert(displacement == 2, "| address displacement (byte) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op

    // the size is word
    value = 0x28; // a0
    displacement = 0;
    A(0) = 0x22390;
    write_16bit(memory + PC + 2, 0x1689); // 0x1689
    write_16bit(memory + A(0) + 0x1689, 0x1689); // the expected source value
    
    addressing_mode_destination(1, value, &displacement, 0x12345678);
    cr_assert(read_16bit(memory + A(0) + 0x1689) == 0x5678, "| address "
        "displacement (word) | Expect source == 0x1689(A0)(0x%x)",
        read_16bit(memory + A(0) + 0x1689));
    cr_assert(displacement == 2, "| address displacement (word) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op

    // the size is long
    value = 0x2a; // a2
    displacement = 0;
    A(2) = 0x2948b;
    write_16bit(memory + PC + 2, 0xfff4); // -12
    write_32bit(memory + 0x2948b - 12, 0x12828933); // the expected source value
    
    addressing_mode_destination(2, value, &displacement, 0x12345678);
    cr_assert(read_32bit(memory + A(2) - 12) == 0x12345678, "| address "
        "displacement (long) | Expect source == -12(A2)(0x%x)",
        read_32bit(memory + A(2) - 12));
    cr_assert(displacement == 2, "| address displacement (long) | Expect "
        "displacement to be modified (value of the modification: 0x%x)",
        displacement); // no displacement for this op
}

Test(emulator, test_rts, .init=setup_emulator) {
    A(7) = 0;
    write_32bit(memory, 0x12345678); 

    rts();

    cr_assert(PC == 0x12345678, "Expect PC == 0x12345678");
    cr_assert(A(7) == 0x4, "Expect A7 == 0x4");
}

Test(emulator, test_bra, .init=setup_emulator) {
    uint16_t instruction = 0x6000;
    
    // test word 
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0x00f0);
    bra(instruction);

    cr_assert(PC == 0x600, "Expect the valid new address (word >) => %x", PC);

    // test word neg
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0xfff0);
    bra(instruction);

    cr_assert(PC == 0x500, "Expect the valid new address (word <) => %x", PC);

    // test byte
    PC = 0x50e;
    instruction = 0x6030;
    
    bra(instruction);

    cr_assert(PC == 0x540, "Expect the valid new address (byte >) => %x", PC);

    // test byte neg
    PC = 0x50e;
    instruction = 0x60f0;
    
    bra(instruction);

    cr_assert(PC == 0x500, "Expect the valid new address (byte >) => %x", PC);
}

Test(emulator, test_bsr, .init=setup_emulator) {
    uint16_t instruction = 0x6100;
    
    // test word
    A(7) = 0x4;
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0x00f0);
    bsr(instruction);

    cr_assert(A(7) == 0x0, "Expect a push on the stack.");
    cr_assert(PC == 0x600, "Expect the valid new address => %x", PC);
    cr_assert(0x512 == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));


    // test word neg
    A(7) = 0x4;
    PC = 0x50e;
    
    write_16bit(memory + PC + 2, 0xfff0);
    bsr(instruction);

    cr_assert(A(7) == 0x0, "Expect a push on the stack.");
    cr_assert(PC == 0x500, "Expect the valid new address => %x", PC);
    cr_assert(0x512 == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));

    // test byte
    A(7) = 0x4;
    PC = 0x50e;
    instruction = 0x6130;
    
    bsr(instruction);

    cr_assert(A(7) == 0x0, "Expect a push on the stack.");
    cr_assert(PC == 0x540, "Expect the valid new address => %x", PC);
    cr_assert(0x510 == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));

    // test byte neg
    A(7) = 0x4;
    PC = 0x50e;
    instruction = 0x61f0;
    
    bsr(instruction);

    cr_assert(A(7) == 0x0, "Expect a push on the stack.");
    cr_assert(PC == 0x500, "Expect the valid new address => %x", PC);
    cr_assert(0x510 == read_32bit(memory), "Expect the valid return address => %x",
        read_16bit(memory));
}

Test(emulator, test_bcc, .init=setup_emulator) {
    uint32_t instruction;

    // bhi
    PC = 0x50e;
    ZERO = 1;
    instruction = 0x6200;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bhi.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6200;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bhi.w jump <) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6200;
    write_16bit(memory + PC + 2, 0x00f0);
    bcc(instruction);
    cr_assert(PC == 0x600, "Error on the (bhi.w jump >) PC => %x", PC);

    PC = 0x50e;
    ZERO = 1;
    instruction = 0x62f0;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x510, "Error on the (bhi.b jump <) PC => %x", PC);


    PC = 0x50e;
    ZERO = 0;
    instruction = 0x62f0;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bhi.b jump <) PC => %x", PC);

    // bls
    PC = 0x50e;
    instruction = 0x6300;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bls.w no jump) PC => %x", PC);

    PC = 0x50e;
    CARRY = 1;
    NEGATIVE = 1;
    instruction = 0x6300;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bls.w jump <) PC => %x", PC);

    // bcc
    PC = 0x50e;
    CARRY = 1;
    NEGATIVE = 0;
    instruction = 0x6400;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bcc.w no jump) PC => %x", PC);

    PC = 0x50e;
    CARRY = 0;
    NEGATIVE = 1;
    instruction = 0x6400;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bcc.w jump <) PC => %x", PC);

    // bcs
    PC = 0x50e;
    CARRY = 0;
    instruction = 0x6500;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bcs.w no jump) PC => %x", PC);

    PC = 0x50e;
    CARRY = 1;
    instruction = 0x6500;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bcs.w jump <) PC => %x", PC);

    // bne
    PC = 0x50e;
    ZERO = 1;
    instruction = 0x6600;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bne.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6600;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bne.w jump <) PC => %x", PC);

    // beq
    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6700;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (beq.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 1;
    instruction = 0x6700;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (beq.w jump <) PC => %x", PC);

    // bvc
    PC = 0x50e;
    OVERFLOW = 1;
    instruction = 0x6800;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bvc.w no jump) PC => %x", PC);

    PC = 0x50e;
    OVERFLOW = 0;
    instruction = 0x6800;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bvc.w jump <) PC => %x", PC);

    // bvs
    PC = 0x50e;
    OVERFLOW = 0;
    instruction = 0x6900;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bvs.w no jump) PC => %x", PC);

    PC = 0x50e;
    OVERFLOW = 1;
    instruction = 0x6900;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bvs.w jump <) PC => %x", PC);

    // bpl
    PC = 0x50e;
    NEGATIVE = 1;
    instruction = 0x6a00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bpl.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 0;
    instruction = 0x6a00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bpl.w jump <) PC => %x", PC);

    // bmi
    PC = 0x50e;
    NEGATIVE = 0;
    instruction = 0x6b00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bmi.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 1;
    instruction = 0x6b00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bmi.w jump <) PC => %x", PC);

    // bge
    PC = 0x50e;
    NEGATIVE = 1;
    OVERFLOW = 0;
    instruction = 0x6c00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bge.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6c00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bge.w jump <) PC => %x", PC);

    // blt
    PC = 0x50e;
    NEGATIVE = 1;
    OVERFLOW = 1;
    instruction = 0x6d00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (blt.w no jump) PC => %x", PC);

    PC = 0x50e;
    NEGATIVE = 0;
    OVERFLOW = 1;
    instruction = 0x6d00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (blt.w jump <) PC => %x", PC);

    // bgt
    PC = 0x50e;
    ZERO = 1;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6e00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (bgt.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6e00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (bgt.w jump <) PC => %x", PC);

    // ble
    PC = 0x50e;
    ZERO = 1;
    NEGATIVE = 0;
    OVERFLOW = 0;
    instruction = 0x6f00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x512, "Error on the (ble.w no jump) PC => %x", PC);

    PC = 0x50e;
    ZERO = 0;
    instruction = 0x6f00;
    write_16bit(memory + PC + 2, 0xfff0);
    bcc(instruction);
    cr_assert(PC == 0x500, "Error on the (ble.w jump <) PC => %x", PC);

}

Test(emulator, test_cmp, .init=setup_emulator) {
    uint32_t instruction;

    // test data register .b
    instruction = 0xb200;

    PC = 0x50c;
    D(0) = 0x2; 
    D(1) = 0x1;
    cmp(instruction);
    cr_assert(PC == 0x50e, "Error on the PC => %x", PC);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    D(1) = 0x2;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_assert(ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    D(1) = -0x1;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    D(1) = 1;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 1; 
    D(1) = -128;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -1; 
    D(1) = -128;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    D(1) = -1;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data register .w
    instruction = 0xb240;
    PC = 0x50c;
    D(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data register .l 
    instruction = 0xb280;
    PC = 0x50c;
    D(0) = -0x1; 
    D(1) = 0x1;
    cmp(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
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
    cr_assert(PC == 0x50e, "Error on the PC => %x", PC);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    A(1) = 0x2;
    cmpa(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    A(1) = 0x1;
    cmpa(instruction);
    cr_assert(ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -0x1; 
    A(1) = 0x1;
    cmpa(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 0x1; 
    A(1) = -0x1;
    cmpa(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    A(1) = 1;
    cmpa(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = 1; 
    A(1) = -128;
    cmpa(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -1; 
    A(1) = -128;
    cmpa(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x50c;
    D(0) = -128; 
    A(1) = -1;
    cmpa(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .w) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .w) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .w) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .w) => "
        "OVERFLOW = 0x%x", OVERFLOW);
}


Test(emulator, test_cmpi, .init=setup_emulator) {
    uint32_t instruction;

    // test data register .b
    instruction = 0x0c01;

    PC = 0x506;
    write_16bit(memory + PC + 2, (uint8_t)0x2);
    D(1) = (uint8_t)0x1;
    cmpi(instruction);
    cr_assert(PC == 0x508, "Error on the PC => %x", PC);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (uint8_t)0x1);
    D(1) = (uint8_t)0x2;
    cmpi(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (uint8_t)0x1);
    D(1) = (uint8_t)0x1;
    cmpi(instruction);
    cr_assert(ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (int8_t)-0x1);
    D(1) = (uint8_t)0x1;
    cmpi(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (uint8_t)0x1);
    D(1) = (int8_t)-0x1;
    cmpi(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (int8_t)-128);
    D(1) = (uint8_t)1;
    cmpi(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (uint8_t)0x1);
    D(1) = (int8_t)-128;
    cmpi(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (int8_t)-1);
    D(1) = (int8_t)-128;
    cmpi(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x506;
    write_16bit(memory + PC + 2, (int8_t)-128);
    D(1) = (int8_t)-1;
    cmpi(instruction);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);
    
    // test data register .w
    instruction = 0x0c41;
    PC = 0x506;
    write_16bit(memory + PC + 2, (int8_t)-0x1);
    D(1) = (uint8_t)0x1;
    cmpi(instruction);
    cr_assert(PC == 0x508, "Error on the PC => %x", PC);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test data register .l 
    instruction = 0x0c81;
    PC = 0x506;
    write_32bit(memory + PC + 2, (int8_t)-0x1);
    D(1) = (uint8_t)0x1;
    cmpi(instruction);
    cr_assert(PC == 0x50a, "Error on the PC => %x", PC);
    cr_assert(!ZERO, "Error on the status register (data register .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (data register .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (data register .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (data register .b) => "
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
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = 2; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = 1; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -1; 
    A(1) = 0x8000;
    memory[0x8000] = 1; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = 1; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = 1; 
    A(1) = 0x8000;
    memory[0x8000] = -128; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -1; 
    A(1) = 0x8000;
    memory[0x8000] = -128; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7001, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8001, "Error on the address A1 => %x", A(1));
    cr_assert(!ZERO, "Error on the status register (address .b) => "
        "ZERO = 0x%x", ZERO);
    cr_assert(!NEGATIVE, "Error on the status register (address .b) => "
        "NEGATIVE = 0x%x", NEGATIVE);
    cr_assert(!CARRY, "Error on the status register (address .b) => "
        "CARRY = 0x%x", CARRY);
    cr_assert(!OVERFLOW, "Error on the status register (address .b) => "
        "OVERFLOW = 0x%x", OVERFLOW);

    // test incrementation .w
    instruction = 0xb348;
    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7002, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8002, "Error on the address A1 => %x", A(1));
  
    // test incrementation .l
    instruction = 0xb388;
    PC = 0x514;
    A(0) = 0x7000; 
    memory[0x7000] = -128; 
    A(1) = 0x8000;
    memory[0x8000] = -1; 
    cmpm(instruction);
    cr_assert(PC == 0x516, "Error on the PC => %x", PC);
    cr_assert(A(0) == 0x7004, "Error on the address A0 => %x", A(0));
    cr_assert(A(1) == 0x8004, "Error on the address A1 => %x", A(1));
}

Test(emulator, test_adda, .init=setup_emulator) {
    uint16_t instruction;

    // test data register .w 
    instruction = 0xd8c2;

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666;
    adda(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(A(4) == 0x698, "Error on the destination address for adda: "
        "(data register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666666;
    adda(instruction);
    cr_assert(A(4) == 0x6698, "Error on the destination address for adda: "
        "(data register .w) => A4 = 0x%x", A(4));

    // test data register .l 
    instruction = 0xd9c2;

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666;
    adda(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(A(4) == 0x698, "Error on the destination address for adda: "
        "(data register .l) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x32; 
    D(2) = 0x666666;
    adda(instruction);
    cr_assert(A(4) == 0x666698, "Error on the destination address for adda: "
        "(data register .l) => A4 = 0x%x", A(4));
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
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(D(1) == 0x12345621, "Error on the destination address for move: "
        "(data register .b) => D1 = 0x%x", D(1));


   // test data register .w
    instruction = 0x3202;

    //move.w		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    D(2) = 0x87654321;
    move(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(D(1) == 0x12344321, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x2202;

    //move.l		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    D(2) = 0x87654321;
    move(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(D(1) == 0x87654321, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));
}

Test(emulator, test_moveq, .init=setup_emulator) {
    uint16_t instruction;
    
    // test data register .b
    instruction = 0x7208;

    //move.b		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(D(1) == 0x8, "Error on the destination address for move: "
        "(data register .b) => D1 = 0x%x", D(1));


   // test data register .w
    instruction = 0x7240;

    //move.w		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(D(1) == 0x40, "Error on the destination address for move: "
    "(data register .w) => D1 = 0x%x", D(1));


    // test data register .l 
    instruction = 0x727f;

    //move.l		D2,D1

    PC = 0x500;
    D(1) = 0x12345678;
    moveq(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(D(1) == 0x7f, "Error on the destination address for move: "
        "(data register .l) => D1 = 0x%x", D(1));
}

Test(emulator, test_movea, .init=setup_emulator) {
    uint16_t instruction;

    // test data register .w 
    instruction = 0x3842;
    PC = 0x500;
    A(4) = 0x5678;
    D(2) = 0x87654321;
    movea(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(A(4) == 0x00004321, "Error on the destination address for movea: "
        "(data register .w) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x12345678;
    D(2) = 0x87654321;
    movea(instruction);
    cr_assert(A(4) == 0x00004321, "Error on the destination address for movea: "
        "(data register .w) => A4 = 0x%x", A(4));

    // test data register .l 
    instruction = 0x2842;

    PC = 0x500;
    A(4) = 0x12345678;
    D(2) = 0x4321;
    movea(instruction);
    cr_assert(PC == 0x502, "Error on the PC => %x", PC);
    cr_assert(A(4) == 0x4321, "Error on the destination address for movea: "
        "(data register .l) => A4 = 0x%x", A(4));

    PC = 0x500;
    A(4) = 0x12345678;
    D(2) = 0x87654321;
    movea(instruction);
    cr_assert(A(4) == 0x87654321, "Error on the destination address for movea: "
        "(data register .l) => A4 = 0x%x", A(4));
}

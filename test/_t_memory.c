#include <stdint.h>
#include <criterion/criterion.h>

#include "../src/memory.h"

Test(memory, read_16bit) {
    uint8_t memory[2];

    memory[0] = 0xF0;
    memory[1] = 0x34;

    uint32_t tmp = read_16bit(memory);

    cr_expect(tmp == 0xF034, "Expect 0xF034 => 0x%x", tmp);
}

Test(memory, read_32bit) {
    uint8_t memory[4];

    memory[0] = 0xF0;
    memory[1] = 0x34;
    memory[2] = 0x99;
    memory[3] = 0x56;

    uint32_t tmp = read_32bit(memory);

    cr_expect(tmp == 0xF0349956, "Expect 0xF0349956 => 0x%x", tmp);
}

Test(memory, write_8bit) {
    uint8_t memory[4];
    uint32_t nb = 0x89ABCDEF;

    write_8bit_memory(0, nb);

    cr_expect(memory[0] == 0xEF, "Expect 0xEF => 0x%x", *memory);
}

Test(memory, write_16bit) {
    uint8_t memory[4];
    uint32_t nb = 0x89ABCDEF;

    write_16bit(memory, nb);

    cr_expect(memory[0] == 0xCD && memory[1] == 0xEF,
        "Expect 0xCDEF => 0x%x%x", memory[0], memory[1]);
}

Test(memory, write_32bit) {
    uint8_t memory[4];
    uint32_t nb = 0x89ABCDEF;

    write_32bit(memory, nb);

    cr_expect(memory[0] == 0x89 && memory[1] == 0xAB && memory[2] == 0xCD
        && memory[3] == 0xEF, "Expect 0x89ABCDEF=> 0x%x%x%x%x",
        memory[0], memory[1], memory[2], memory[3]);
}

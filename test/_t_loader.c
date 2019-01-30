#include <stdint.h>
#include <stdlib.h>
#include <criterion/criterion.h>

#include "../src/loader.h"

extern uint8_t *memory;

void setup(void) {
    memory = calloc(16777216, 1);
}

Test(loader, test_copy_raw_data, .init=setup) {
    int tmp = copy_raw_data("484452", 3, 0);
     
    cr_expect(memory[0] == 0x48, "Expect 0x48 for the first byte");
    cr_expect(memory[1] == 0x44, "Expect 0x44 for the second byte");
    cr_expect(memory[2] == 0x52, "Expect 0x42 for the third byte");
    cr_expect((~(0x06 + tmp) & 0xFF) == 0x1B, "Expect 0x1B as checksum");

    tmp = copy_raw_data("285F245F2212226A000424290008237C", 16, 0);
    cr_expect((~(0x13 + tmp) & 0xFF) == 0x2A, "Expect 0x2A as checksum");

    tmp = copy_raw_data("00020008000826290018538123410018", 16, 0);
    cr_expect((~(0x13 + 0x10 + tmp) & 0xFF) == 0x13, "Expect 0x13 as checksum");
}

Test(loader, load_line, .init=setup) {
    int tmp = load_line("S214000000285F245F2212226A000424290008237C29");    

    cr_expect(tmp == 0, "Expect 0 as output.");
    
    cr_expect(memory[0] == 0x28, "Expect 0x28 for the 0 byte");
    cr_expect(memory[1] == 0x5F, "Expect 0x5F for the 1 byte");
    cr_expect(memory[2] == 0x24, "Expect 0x24 for the 2 byte");
    cr_expect(memory[3] == 0x5F, "Expect 0x5F for the 3 byte");
    cr_expect(memory[4] == 0x22, "Expect 0x22 for the 4 byte");
    cr_expect(memory[5] == 0x12, "Expect 0x12 for the 5 byte");
    cr_expect(memory[6] == 0x22, "Expect 0x22 for the 6 byte");
    cr_expect(memory[7] == 0x6A, "Expect 0x6A for the 7 byte");
    cr_expect(memory[8] == 0x00, "Expect 0x00 for the 8 byte");
    cr_expect(memory[9] == 0x04, "Expect 0x04 for the 9 byte");
    cr_expect(memory[10] == 0x24, "Expect 0x24 for the 10 byte");
    cr_expect(memory[11] == 0x29, "Expect 0x29 for the 11 byte");
    cr_expect(memory[12] == 0x00, "Expect 0x00 for the 12 byte");
    cr_expect(memory[13] == 0x08, "Expect 0x08 for the 13 byte");
    cr_expect(memory[14] == 0x23, "Expect 0x23 for the 14 byte");
    cr_expect(memory[15] == 0x7C, "Expect 0x7C for the 15 byte");
}

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
}

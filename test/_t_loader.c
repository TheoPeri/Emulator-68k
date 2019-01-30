#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "../src/loader.h"

extern uint8_t *memory;

void setup(void) {
    memory = calloc(16777216, 1);
}

Test(loader, test_checksum, .init=setup) {
    uint8_t tmp;
    tmp = compute_checksum("060000484452", 6);
    cr_expect(tmp == 0x1B, "Expect 0x1B as checksum => 0x%hhx", tmp);

    tmp = compute_checksum("130000285F245F2212226A000424290008237C", 19);
    cr_expect(tmp == 0x2A, "Expect 0x2A as checksum => 0x%hhx", tmp);

    tmp = compute_checksum("13001000020008000826290018538123410018", 19);
    cr_expect(tmp == 0x13, "Expect 0x13 as checksum => 0x%hhx", tmp);
}

Test(loader, test_copy_raw_data, .init=setup) {
    copy_raw_data("484452", 3, 0);
     
    cr_expect(memory[0] == 0x48, "Expect 0x48 for the first byte");
    cr_expect(memory[1] == 0x44, "Expect 0x44 for the second byte");
    cr_expect(memory[2] == 0x52, "Expect 0x42 for the third byte");

    copy_raw_data("285F245F2212226A000424290008237C", 16, 0);
    copy_raw_data("00020008000826290018538123410018", 16, 0);
}

Test(loader, load_line, .init=setup) {
    uint8_t byte;
    int tmp = load_line("S1130000285F245F2212226A000424290008237C2A");    

    cr_expect(tmp == 0, "Expect 0 as output.\n");
    char *s = "285F245F2212226A000424290008237C"; 

    for (unsigned i = 0; i < 16; ++i) {
        sscanf(s + 2 * i, "%2hhx", &byte);
        cr_expect(memory[i] == byte, "Expect 0x%hhx for the %u byte, "
            "received: %hhx", byte, i, memory[i]);
    }
}

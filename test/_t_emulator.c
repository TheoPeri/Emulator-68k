#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "../src/emulator.h"

Test(emulator, is_ori) {
    for (uint16_t i = 0; i < 0xff; ++i) {
        cr_assert(is_ori(i));
    }
}

Test(emulator, is_andi) {
    for (uint16_t i = 0; i < 0xff; ++i) {
        cr_assert(is_andi(i + 0x200));
    }
}

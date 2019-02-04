#include <stdint.h>
#include <stdlib.h>

#include "emulator.h"
#include "memory.h"

extern uint8_t *memory;

int init() {
    memory = calloc(16777220, sizeof(uint8_t));

    return 0;
}

int shutdown() {
    free(memory);

    return 0;
}

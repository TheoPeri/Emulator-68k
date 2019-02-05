#include <stdint.h>
#include <stdlib.h>

#include "emulator.h"
#include "memory.h"

extern uint32_t PC;
extern uint8_t *memory;

int init() {
    memory = calloc(16777220, sizeof(uint8_t));

    return 0;
}

int shutdown() {
    free(memory);

    return 0;
}

int next_instruction() {
    uint16_t current_operation;

    current_operation = read_16bit(memory + PC);
    
    switch(current_operation >> 14) {
        case 0b01:
            switch(current_operation >> 12 & 0b11){
                case 0b00:
                    switch(current_operation >> 10 & 0b11){
                        case 0b10:
                            switch(current_operation >> 8 & 0b11){
                                case 0b10:
                                    break;
                                default:
                                    goto WARNING;
                            }
                        default:
                            goto WARNING;
                    }
                    break;
            }
            break;

        default:
            goto WARNING;    
    }

    return 0;

    WARNING:
        fprintf(2, "Warning instruction not implemented => %hx",
            current_operation);
        return -1;
}

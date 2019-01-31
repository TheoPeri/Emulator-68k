#include <stdint.h>
#include <stdlib.h>

#include "emulator.h"

// register
uint32_t PC;

uint32_t d0;
uint32_t d1;
uint32_t d2;
uint32_t d3;
uint32_t d4;
uint32_t d5;
uint32_t d6;
uint32_t d7;

uint32_t a0;
uint32_t a1;
uint32_t a2;
uint32_t a3;
uint32_t a4;
uint32_t a5;
uint32_t a6;
uint32_t a7;

// memory
uint8_t *memory;

// status register
#define CARRY                           status_register[0]
#define OVERFLOW                        status_register[1]
#define ZERO                            status_register[2]
#define NEGATIVE                        status_register[3]
#define EXTEND                          status_register[4]
#define INTERRUPT_PRIORITY_MASK0        status_register[8]
#define INTERRUPT_PRIORITY_MASK1        status_register[9]
#define INTERRUPT_PRIORITY_MASK2        status_register[10]
#define MASTER_INTERRUPT_STATE          status_register[12]
#define SUPERVISOR_STATE                status_register[13]
#define TRACE_MODE0                     status_register[14]
#define TRACE_MODE1                     status_register[15]
 
uint8_t status_register[16];

int init() {
    memory = calloc(16777220, sizeof(uint8_t));

    return 0;
}

int shutdown() {
    free(memory);

    return 0;
}

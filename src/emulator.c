#include <stdint.h>

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
uint8_t memory[16777220];

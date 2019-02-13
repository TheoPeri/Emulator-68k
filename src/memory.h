#ifndef _MEMORY_H
#define _MEMORY_H

// memory
uint8_t *memory;

// registers
uint32_t PC;

// status registers
int8_t status_registers[6];

// data and address registers
uint32_t registers[17];

#define CARRY                           status_registers[0]
#define OVERFLOW                        status_registers[1]
#define ZERO                            status_registers[2]
#define NEGATIVE                        status_registers[3]
#define EXTEND                          status_registers[4]
#define SUPERVISOR_STATE                status_registers[5]

#define D(i) registers[i]

#define A(i) (registers + 8)[i]

uint32_t get_stack_ptr();

uint32_t read_16bit(uint8_t *address);

uint32_t read_32bit(uint8_t *address);

void write_8bit(uint8_t *address, uint32_t value);

void write_16bit(uint8_t *address, uint32_t value);

void write_32bit(uint8_t*address, uint32_t value);
#endif

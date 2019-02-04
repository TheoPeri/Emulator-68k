#ifndef _MEMORY_H
#define _MEMORY_H

uint32_t read_16bit(uint8_t *address);

uint32_t read_32bit(uint8_t *address);

void write_8bit(uint8_t *address, uint32_t value);

void write_16bit(uint8_t *address, uint32_t value);

void write_32bit(uint8_t*address, uint32_t value);
#endif

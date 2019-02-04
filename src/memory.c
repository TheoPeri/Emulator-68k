#include <stdint.h>

#include "memory.h"

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

// operation on memory

/**
 * @brief Read 16 bit in memory (big endian => low endian).
 *
 * @param address The starting address of the big endian memory.
 *
 * @return The number in low endian.
 */
inline uint32_t read_16bit(uint8_t *address) {
    return (uint32_t)address[0]<<8 | (uint32_t)address[1];
}

/**
 * @brief Read 32 bit in memory (big endian => low endian).
 *
 * @param address The starting address of the big endian memory.
 *
 * @return The number in low endian.
 */
inline uint32_t read_32bit(uint8_t *address) {
    return (uint32_t)address[0]<<24 | (uint32_t)address[1]<<16 
        | (uint32_t)address[2]<<8 | (uint32_t)address[3];
}

/**
 * @brief Write 8 bit in memory (low endian => big endian).
 *
 * @param address The starting address of the big endian memory.
 * @param value The value in low endian.
 */
inline void write_8bit(uint8_t *address, uint32_t value) {
    address[0] = value & 0xFF;
}

/**
 * @brief Write 16 bit in memory (low endian => big endian).
 *
 * @param address The starting address of the big endian memory.
 * @param value The value in low endian.
 */
inline void write_16bit(uint8_t *address, uint32_t value) {
    address[0] = (value & 0xFF00)>>8;
    address[1] = value & 0xFF;
}

/**
 * @brief Write 32 bit in memory (low endian => big endian).
 *
 * @param address The starting address of the big endian memory.
 * @param value The value in low endian.
 */
inline void write_32bit(uint8_t*address, uint32_t value) {
    address[0] = (value & 0xFF000000)>>24;
    address[1] = (value & 0xFF0000)>>16;
    address[2] = (value & 0xFF00)>>8;
    address[3] = value & 0xFF;
}

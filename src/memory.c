#include <stdint.h>

#include "memory.h"

// memory
uint8_t *memory;

// registers
uint32_t PC;

// status registers
int8_t status_registers[6];

#define CARRY                           status_registers[0]
#define OVERFLOW                        status_registers[1]
#define ZERO                            status_registers[2]
#define NEGATIVE                        status_registers[3]
#define EXTEND                          status_registers[4]
#define SUPERVISOR_STATE                status_registers[5]

// data and address registers
uint32_t registers[17];

#define D0 regiters[0]
#define D1 regiters[1]
#define D2 regiters[2]
#define D3 regiters[3]
#define D4 regiters[4]
#define D5 regiters[5]
#define D6 regiters[6]
#define D7 regiters[7]

#define A0 regiters[8]
#define A1 regiters[9]
#define A2 regiters[10]
#define A3 regiters[11]
#define A4 regiters[12]
#define A5 regiters[13]
#define A6 regiters[14]
#define A7 a7()

uint32_t a7() {
    return SUPERVISOR_STATE ? registers[15] : registers[16];    
}

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

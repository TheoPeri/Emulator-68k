#include <stdint.h>
#include <stddef.h>

#include "memory.h"

uint8_t *memory = NULL;
uint32_t PC;
int8_t status_registers[6];
uint32_t registers[17];

/**
* @brief Get index in the registers array of stack ptr
*
* @return The index in the registers array of stack ptr.
*/
uint32_t get_stack_ptr (){
    return SUPERVISOR_STATE ? 16 : 17;    
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
    address[0] = value >> 24;
    address[1] = (value & 0xFF0000)>>16;
    address[2] = (value & 0xFF00)>>8;
    address[3] = value & 0xFF;
}

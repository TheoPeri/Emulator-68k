#include <stdint.h>
#include <stdlib.h>
#include <err.h>

#include "emulator.h"
#include "memory.h"

extern uint32_t PC;
extern uint8_t *memory;

/**
 * @brief Initialize the emulator.
 *
 * @return -1 => error || other => OK
 */
int init() {
    memory = calloc(16777220, sizeof(uint8_t));

    return 0;
}

/**
 * @brief Shutdown the emulator.
 *
 * @return -1 => error || other => OK
 */
int shutdown() {
    free(memory);

    return 0;
}

/**
 * @brief Execute the next instruction.
 *
 * @return -1 => error || other => OK
 */
int next_instruction() {
    uint16_t current_operation;

    current_operation = read_16bit(memory + PC);
    
    warnx("Warning instruction not implemented => %hx", current_operation);
    return -1;
}

/**
 * @brief Test if the current_operation is ori to ccr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_ori_to_ccr(uint16_t current_operation) {
    return current_operation == 0x3c;
}

/**
 * @brief Test if the current_operation is ori to sr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_ori_to_sr(uint16_t current_operation) {
    return current_operation == 0x7c;
}

/**
 * @brief Test if the current_operation is ori.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_ori(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x0;
}

/**
 * @brief Test if the current_operation is andi to ccr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_andi_to_ccr(uint16_t current_operation) {
    return current_operation == 0x23c;
}

/**
 * @brief Test if the current_operation is andi to sr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_andi_to_sr(uint16_t current_operation) {
    return current_operation == 0x27c;
}

/**
 * @brief Test if the current_operation is andi.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_andi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x200;
}

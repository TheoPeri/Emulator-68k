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

/**
 * @brief Test if the current_operation is subi.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_subi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x400;
}

/**
 * @brief Test if the current_operation is addi.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_addi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x600;
}

/**
 * @brief Test if the current_operation is eori to ccr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_eori_to_ccr(uint16_t current_operation) {
    return current_operation == 0xa3c;
}

/**
 * @brief Test if the current_operation is eori to sr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_eori_to_sr(uint16_t current_operation) {
    return current_operation == 0xa7c;
}

/**
 * @brief Test if the current_operation is eori.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_eori(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0xa00;
}

/**
 * @brief Test if the current_operation is cmpi.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_cmpi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0xc00;
}

/**
 * @brief Test if the current_operation is btst.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_btst(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x800
        || (0xf1c0 & current_operation) == 0x100;
}

/**
 * @brief Test if the current_operation is bchg.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_bchg(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x840
        || (0xf1c0 & current_operation) == 0x140;
}

/**
 * @brief Test if the current_operation is bclr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_bclr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x880
        || (0xf1c0 & current_operation) == 0x180;
}

/**
 * @brief Test if the current_operation is bset.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_bset(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x8c0
        || (0xf1c0 & current_operation) == 0x1c0;
}

/**
 * @brief Test if the current_operation is movep.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_movep(uint16_t current_operation) {
    return (0xf138 & current_operation) == 0x108;
}

/**
 * @brief Test if the current_operation is movea.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_movea(uint16_t current_operation) {
    return (0xc1c0 & current_operation) == 0x40;
}

/**
 * @brief Test if the current_operation is move.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_move(uint16_t current_operation) {
    return (0xc000 & current_operation) == 0x0;
}

/**
 * @brief Test if the current_operation is move from sr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_move_from_sr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x40c0;
}

/**
 * @brief Test if the current_operation is move to ccr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_move_to_ccr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x44c0;
}

/**
 * @brief Test if the current_operation is move to sr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_move_to_sr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x46c0;
}

/**
 * @brief Test if the current_operation is negx.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_negx(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4000;
}

/**
 * @brief Test if the current_operation is clr.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_clr(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4200;
}

/**
 * @brief Test if the current_operation is neg.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_neg(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4400;
}

/**
 * @brief Test if the current_operation is not.
 *
 * @param current_operation The current opcode.
 *
 * @return 0 => False || other => True 
 */
inline int is_not(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4600;
}

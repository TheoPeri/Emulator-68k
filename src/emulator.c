#include <stdint.h>
#include <stdlib.h>
#include <err.h>

#include <stdio.h>
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
 * @brief Parse the source operand.
 *
 * @param size The size mode 0 => b, 1 => w, 2 => l
 * @param value The M and Xn value.
 * @param displacement A ptr on the diplacement of the current instruction.
 *
 * @return the value of the source operand.
 */
inline uint32_t addressing_mode_source(
    uint8_t size,
    uint8_t value,
    uint32_t *displacement
) {
    uint32_t source = -1;
    uint32_t tmp;
    uint8_t reg = value & 0x7;

    // mode
    switch (value & 0x38) {
        case 0x00: // data register
            switch (size) {
                case 0x0:
                    source = D(reg) & 0xff;
                    break;
                case 0x1:
                    source = D(reg) & 0xffff; 
                    break;
                case 0x2:
                    source = D(reg); 
                    break;
            }
            break;
        case 0x08: // address register
            source = A(reg); 
            break;
        case 0x10: // address
            switch (size) {
                case 0x0:
                    source = memory[A(reg)];
                    break;
                case 0x1:
                    source = read_16bit(memory + A(reg));
                    break;
                case 0x2:
                    source = read_32bit(memory + A(reg));
                    break;
            }
            break;
        case 0x18: // address postincrement
            switch (size) {
                case 0x0:
                    source = memory[A(reg)];
                    ++A(reg);
                    break;
                case 0x1:
                    source = read_16bit(memory + A(reg));
                    A(reg) += 2;
                    break;
                case 0x2:
                    source = read_32bit(memory + A(reg));
                    A(reg) += 4;
                    break;
            }
            break;
        case 0x20: // address predecrement
            switch (size) {
                case 0x0:
                    --A(reg);
                    source = memory[A(reg)];
                    break;
                case 0x1:
                    A(reg) -= 2;
                    source = read_16bit(memory + A(reg));
                    break;
                case 0x2:
                    A(reg) -= 4;
                    source = read_32bit(memory + A(reg));
                    break;
            }
            break;
        case 0x28: // address displacement
            tmp = A(reg) + (int16_t)read_16bit(memory + PC + 2);
            switch (size) {
                case 0x0:
                    source = memory[tmp];
                    break;
                case 0x1:
                    source = read_16bit(memory + tmp);
                    break;
                case 0x2:
                    source = read_32bit(memory + tmp);
                    break;
            }

            *displacement += 2;
            break;
        case 0x38: // immediate
            switch (size) {
                case 0x0:
                    source = read_16bit(memory + PC + 2);
                    *displacement += 2;
                    break;
                case 0x1:
                    source = read_16bit(memory + PC + 2);
                    *displacement += 2;
                    break;
                case 0x2:
                    source = read_32bit(memory + PC + 2);
                    *displacement += 4;
                    break;
            }
    }

    return source;
}

/**
 * @brief Parse the source operand with read only.
 *
 * @param size The size mode 0 => b, 1 => w, 2 => l
 * @param value The M and Xn value.
 * @param displacement A ptr on the diplacement of the current instruction.
 *
 * @return the value of the source operand.
 */
inline uint32_t addressing_mode_source_ro(
    uint8_t size,
    uint8_t value
) {
    uint32_t source = -1;
    uint32_t tmp;
    uint8_t reg = value & 0x7;

    // mode
    switch (value & 0x38) {
        case 0x00: // data register
            switch (size) {
                case 0x0:
                    source = D(reg) & 0xff;
                    break;
                case 0x1:
                    source = D(reg) & 0xffff; 
                    break;
                case 0x2:
                    source = D(reg); 
                    break;
            }
            break;
        case 0x08: // address register
            source = A(reg); 
            break;
        case 0x28: // address displacement
            tmp = A(reg) + (int16_t)read_16bit(memory + PC + 2);
            switch (size) {
                case 0x0:
                    source = memory[tmp];
                    break;
                case 0x1:
                    source = read_16bit(memory + tmp);
                    break;
                case 0x2:
                    source = read_32bit(memory + tmp);
                    break;
            }
            break;
        case 0x38: // immediate
            switch (size) {
                case 0x0:
                    source = read_16bit(memory + PC + 2);
                    break;
                case 0x1:
                    source = read_16bit(memory + PC + 2);
                    break;
                case 0x2:
                    source = read_32bit(memory + PC + 2);
                    break;
            }
            break;
        default:
            if ((value & 0x38) == 0x10 || (value & 0x38) == 0x18
                || (value & 0x38) == 0x20) {
                switch (size) {
                    case 0x0:
                        source = memory[A(reg)];
                        break;
                    case 0x1:
                        source = read_16bit(memory + A(reg));
                        break;
                    case 0x2:
                        source = read_32bit(memory + A(reg));
                        break;
                }
            }
            break;
    }

    return source;
}

/**
 * @brief Parse the destination operand.
 *
 * @param size The size mode 0 => b, 1 => w, 2 => l
 * @param value The M and Xn value (IN THIS ORDER).
 * @param displacement A ptr on the diplacement of the current instruction.
 * @param data The data to write on destination operand.
 *
*/
inline void addressing_mode_destination(
    uint8_t size,
    uint8_t value,
    uint32_t *displacement,
    uint32_t data
) {
    uint32_t tmp;
    uint8_t reg = value & 0x7;

    // mode
    switch (value & 0x38) {
        case 0x00: // data register
            switch (size) {
                case 0x0:
                    D(reg) = (D(reg) & 0xffffff00) | (data & 0xff);
                    break;
                case 0x1:
                    D(reg) = (D(reg) & 0xffff0000) | (data & 0xffff);
                    break;
                case 0x2:
                    D(reg) = data;
                    break;
            }
            break;
        case 0x08: // data register
            switch (size) {
                case 0x0:
                    A(reg) = (A(reg) & 0xffffff00) | (data & 0xff);
                    break;
                case 0x1:
                    A(reg) = (A(reg) & 0xffff0000) | (data & 0xffff);
                    break;
                case 0x2:
                    A(reg) = data;
                    break;
            }
            break;

        case 0x10: // address
            switch (size) {
                case 0x0:
                    memory[A(reg)] = data;
                    break;
                case 0x1:
                    write_16bit(memory + A(reg), data);
                    break;
                case 0x2:
                    write_32bit(memory + A(reg), data);
                    break;
            }
            break;
        case 0x18: // address postincrement
            switch (size) {
                case 0x0:
                    memory[A(reg)] = data;
                    ++A(reg);
                    break;
                case 0x1:
                    write_16bit(memory + A(reg),data);
                    A(reg) += 2;
                    break;
                case 0x2:
                    write_32bit(memory + A(reg),data);
                    A(reg) += 4;
                    break;
            }
            break;
        case 0x20: // address predecrement
            switch (size) {
                case 0x0:
                    --A(reg);
                    memory[A(reg)] = data;
                    break;
                case 0x1:
                    A(reg) -= 2;
                    write_16bit(memory + A(reg),data);
                    break;
                case 0x2:
                    A(reg) -= 4;
                    write_32bit(memory + A(reg),data);
                    break;
            }
            break;
        case 0x28: // address displacement
            tmp = A(reg) + (int16_t)read_16bit(memory + PC + 2);
            switch (size) {
                case 0x0:
                    memory[tmp] = data;
                    break;
                case 0x1:
                    write_16bit(memory + tmp, data);
                    break;
                case 0x2:
                    write_32bit(memory + tmp, data);
                    break;
            }

            *displacement += 2;
            break;
    }
}

/**
 * @brief Execute the next instruction.
 *
 * @return -1 => error || other => OK
 */
int next_instruction() {
    uint16_t current_operation = read_16bit(memory + PC);

    // mask
    uint16_t mask_0xf000 = 0xf000 & current_operation;
    uint16_t mask_0xf018 = 0xf018 & current_operation;
    uint16_t mask_0xf0c0 = 0xf0c0 & current_operation;
    uint16_t mask_0xf100 = 0xf100 & current_operation;
    uint16_t mask_0xf130 = 0xf130 & current_operation;
    uint16_t mask_0xf138 = 0xf138 & current_operation;
    uint16_t mask_0xf1c0 = 0xf1c0 & current_operation;
    uint16_t mask_0xf1f0 = 0xf1f0 & current_operation;
    uint16_t mask_0xfec0 = 0xfec0 & current_operation;
    uint16_t mask_0xff00 = 0xff00 & current_operation;
    uint16_t mask_0xffc0 = 0xffc0 & current_operation;
    uint16_t mask_0xfff0 = 0xfff0 & current_operation;
    uint16_t mask_0xfff8 = 0xfff8 & current_operation;
    
    // hardcoded function
    switch (current_operation) {
        case 0x003c:
            // is_ori_to_ccr
            goto warning;
        case 0x007c:
            // is_ori_to_sr
            goto warning;
        case 0x023c:
            // is_andi_to_ccr
            goto warning;
        case 0x27c:
            // is_andi_to_sr
            goto warning;
        case 0x0a3c:
            // is_eori_to_ccr
            goto warning;
        case 0x0a7c:
            // is_eori_to_sr
            goto warning;
        case 0x4afc:
            // is_illegal
            goto warning;
        case 0x4e70:
            // is_reset
            goto warning;
        case 0x4e71:
            // is_nop
            goto warning;
        case 0x4e72:
            // is_stop
            goto warning;
        case 0x4e73:
            // is_rte
            goto warning;
        case 0x4e75:
            // is_rts
            goto warning;
        case 0x4e76:
            // is_trapv
            goto warning;
        case 0x4e77:
            // is_rtr
            goto warning;
    }

    switch(mask_0xff00) {
        case 0x0000:
            // is_ori
            goto warning;
        case 0x0200:
            // is_andi
            goto warning;
        case 0x0400:
            // is_subi
            goto warning;
        case 0x0600:
            // is_addi
            goto warning;
        case 0x0a00:
            // is_eori
            goto warning;
        case 0x0c00:
            // is_cmpi
            goto warning;
    }

    if (mask_0xffc0 == 0x0800 || mask_0xf1c0 == 0x0100) {
        // is_btst
        goto warning;
    }

    if (mask_0xffc0 == 0x0840 || mask_0xf1c0 == 0x0140) {
        // is_bchg
        goto warning;
    }

    if (mask_0xffc0 == 0x0880 || mask_0xf1c0 == 0x0180) {
        // is_bclr
        goto warning;
    }

    if (mask_0xffc0 == 0x08c0 || mask_0xf1c0 == 0x01c0) {
        // is_bset
        goto warning;
    }

    if (mask_0xf138 == 0x0108) {
        // is_movep
    }

    if ((0xc1c0 & current_operation) == 0x0040) {
        // is_movea
    }
    
    if ((0xc000 & current_operation) == 0x0000) {
        // is_move
    }

    switch (mask_0xf1c0) {
        case 0x40c0:
            // is_move_from_sr
            goto warning;
        case 0x44c0:
            // is_move_to_ccr
            goto warning;
        case 0x46c0:
            // is_move_to_sr
            goto warning;
    }

    switch (mask_0xff00) {
        case 0x4000:
            // is_negx
            goto warning;
        case 0x4200:
            // is_clr
            goto warning;
        case 0x4400:
            // is_neg
            goto warning;
        case 0x4600:
            // is_not
            goto warning;
    }

    if ((0xffb8 & current_operation) == 0x4880) {
        // is_ext
        goto warning;
    }

    if (mask_0xffc0 == 0x4800) {
        // is_nbcd
        goto warning;
    }

    if ((0xfff8 & current_operation) == 0x4840) {
        // is_swap
        goto warning;
    }

    switch (mask_0xffc0) {
        case 0x4840:
            // is_pea
            goto warning;
        case 0x4ac0:
            // is_tas
            goto warning;
    }

    if (mask_0xff00 == 0x4a00) {
        // is_tst
        goto warning;
    }
    
    if (mask_0xfff0 == 0x4e40) {
        // is_is_trap
        goto warning;
    }

    if (mask_0xfff8 == 0x4e50) {
        // is_link
        goto warning;
    }

    if (mask_0xfff8 == 0x4e58) {
        // is_unlk
        goto warning;
    }

    if (mask_0xfff0 == 0x4e60) {
        // is_move_usp
        goto warning;
    }

    if (mask_0xffc0 == 0x4e80) {
        // is_jsr
        goto warning;
    }

    if (mask_0xffc0 == 0x4ec0) {
        // is_jmp
        goto warning;
    }

    if ((0xfb80 & current_operation) == 0x4880) {
        // is_movem
        goto warning;
    }

    if (mask_0xf1c0 == 0x41c0) {
        // is_lea
        goto warning;
    }

    if (mask_0xf1c0 == 0x4180) {
        // is_chk
        goto warning;
    }
    
    if (mask_0xf100 == 0x5000) {
        // is_addq
        goto warning;
    }

    if (mask_0xf100 == 0x5100) {
        // is_subq
        goto warning;
    }

    if ((0xf0f8 & current_operation) == 0x50c8) {
        // is_dbcc
        goto warning;
    }

    if (mask_0xf0c0 == 0x50c0) {
        // is_scc
        goto warning;
    }

    if (mask_0xff00 == 0x6000) {
        // is_bra
        goto warning;
    }

    if (mask_0xff00 == 0x6100) {
        // is_bsr
        goto warning;
    }

    if (mask_0xf000 == 0x6000) {
        // is_bcc
        goto warning;
    }

    if (mask_0xf100 == 0x7000) {
        // is_moveq
        goto warning;
    }

    if (mask_0xf1c0 == 0x80c0) {
        // is_divu
        goto warning;
    }

    if (mask_0xf1c0 == 0x81c0) {
        // is_divs
        goto warning;
    }

    if (mask_0xf1f0 == 0x8100) {
        // is_sbcd
        goto warning;
    }

    if (mask_0xf000 == 0x8000) {
        // is_or
        goto warning;
    }

    if (mask_0xf130 == 0x9100) {
        // is_subx
        goto warning;
    }

    if (mask_0xf0c0 == 0x90c0) {
        // is_suba
        goto warning;
    }

    if (mask_0xf000 == 0x9000) {
        // is_sub 
        goto warning;
    }

    if (mask_0xf138 == 0xb108) {
        // is_cmpm
        goto warning;
    }

    if (mask_0xf100 == 0xb100) {
        // is_eor
        goto warning;
    }

    if (mask_0xf100 == 0xb000) {
        // is_cmp
        goto warning;
    }

    if (mask_0xf0c0 == 0xb0c0) {
        // is_cmpa
        goto warning;
    }
    
    if (mask_0xf1c0 == 0xc0c0) {
        // is_mulu
        goto warning;
    }

    if (mask_0xf1c0 == 0xc1c0) {
        // is_muls
        goto warning;
    }
    
    if (mask_0xf1f0 == 0xc100) {
        // is_abcd
        goto warning;
    }

    if (mask_0xf130 == 0xc100) {
        // is_exg
        goto warning;
    }

    if (mask_0xf130 == 0xd100) {
        // is_addx
        goto warning;
    }

    if (mask_0xf0c0 == 0xd0c0) {
        // is_adda
        goto warning;
    }

    if (mask_0xf000 == 0xc000) {
        // is_and
        goto warning;
    }

    if (mask_0xf000 == 0xd000) {
        // is_add
        goto warning;
    }

    if (mask_0xfec0 == 0xe0c0 || mask_0xf018== 0xe000) {
        // is_asd
        goto warning;
    }

    if (mask_0xfec0 == 0xe2c0 || mask_0xf018== 0xe008) {
        // is_lsd
        goto warning;
    }

    if (mask_0xfec0 == 0xe4c0 || mask_0xf018== 0xe010) {
        // is_roxd
        goto warning;
    }

    if (mask_0xfec0 == 0xe6c0 || mask_0xf018== 0xe018) {
        // is_rod
        goto warning;
    }

    goto error;

    warning:
        warnx("Warning instruction not implemented => %hx", current_operation);
        return -1;

    error:
        warnx("Unknown instruction not implemented => %hx", current_operation);
        return -1;
}

// When enter in the function pc => current_operation!!!

/**
* @brief Execute the command rts
*
* @return -1 => error || other => OK 
*/
inline int rts() {
    PC = read_32bit(memory + A(7)); 
    A(7) += 4;

    return 0;
}

/**
* @brief Execute the command bra
*
* @return -1 => error || other => OK 
*/
inline int bra(uint16_t current_operation) {
    uint32_t displacement = current_operation & 0xff; 
    
    // bra 
    PC += (displacement ? (int8_t)displacement
        : (int16_t)read_16bit(memory + PC +2)) + 2;

    return 0;
}

/**
* @brief Execute the command bsr
*
* @param current_operation The current operation
*
* @return -1 => error || other => OK 
*/
inline int bsr(uint16_t current_operation) {
    uint32_t displacement = current_operation & 0xff; 
    
    // push address
    A(7) -= 4;
    
    // bra 
    if (displacement) {
        // return address
        write_32bit(memory + A(7), PC + 2);
        PC += (int8_t)displacement + 2;
    } else {
        // return address
        write_32bit(memory + A(7), PC + 4);
        // get the displacement
        PC += (int16_t)read_16bit(memory + PC + 2) + 2; 
    }

    return 0;
}

/**
* @brief Execute the command bcc
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
inline int bcc(uint16_t current_operation) {
    uint32_t byte_operation = current_operation & 0xff;
    uint32_t condition = current_operation & 0xf00;
    
    switch (condition) {
        case 0x200: // bhi
            condition = !ZERO && !CARRY; 
            break;
        case 0x300: // bls
            condition = ZERO || CARRY; 
            break;
        case 0x400: // bcc
            condition = !CARRY; 
            break;
        case 0x500: // bcs
            condition = CARRY; 
            break;
        case 0x600: // bne
            condition = !ZERO;
            break;
        case 0x700: // beq
            condition = ZERO;
            break;
        case 0x800: // bvc
            condition = !OVERFLOW;
            break;
        case 0x900: // bvs
            condition = OVERFLOW;
            break;
        case 0xa00: // bpl
            condition = !NEGATIVE;
            break;
        case 0xb00: // bmi
            condition = NEGATIVE;
            break;
        case 0xc00: // bge
            condition = (NEGATIVE && OVERFLOW) || (!NEGATIVE && !OVERFLOW);
            break;
        case 0xd00: // blt
            condition = NEGATIVE ^ OVERFLOW;
            break;
        case 0xe00: // bgt
            condition = (!ZERO && !NEGATIVE && !OVERFLOW)
                || (!ZERO && NEGATIVE && OVERFLOW);
            break;
        case 0xf00: // ble
            condition = !ZERO || (NEGATIVE ^ OVERFLOW);
            break;
        default:
            return -1;
    }

    if (condition) {
        PC += 2 + (byte_operation ? (int8_t)(byte_operation)
            : (int16_t)read_16bit(memory + PC + 2)); 
    } else {
        PC += byte_operation ? 2 : 4;
    }

    return 0;
}

/**
* @brief Execute the command cmp
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
int cmp(uint16_t current_operation) {
    uint8_t size = (current_operation & 0xc0) >> 6;
    uint8_t shift;
    
    uint32_t displacement = 2;
    uint32_t source = addressing_mode_source(size, 
        current_operation & 0xff, &displacement); 

    uint32_t destination, tmp;

    switch (size) {
        case 0:
            shift = 7;
            destination = D((current_operation & 0xe00) >> 9) & 0xff;
            break;
        case 1:
            shift = 15;
            destination = D((current_operation & 0xe00) >> 9) & 0xffff;
            break;
        case 2:
            shift = 31;
            destination = D((current_operation & 0xe00) >> 9);
            break;
        default:
            return -1;
    }
    // compute diff 
    tmp = destination - source;
    
    ZERO = source == destination; 
    NEGATIVE = (tmp >> shift) & 0x1; // need cast and get high bit

    CARRY = (((source & ~destination) | (tmp & ~destination) | (source &
    tmp)) >> shift) & 0x1; // see doc

    OVERFLOW = (((~source & destination & ~tmp) | (source & ~destination &
    tmp)) >> shift) & 0x1; // see doc

    PC += displacement;

    return 0;
}

/**
* @brief Execute the command cmpa
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
int cmpa(uint16_t current_operation) {
    uint8_t size = (current_operation & 0x100) ? 2 : 1;
    
    uint32_t displacement = 2;
    uint32_t source = addressing_mode_source(size, 
        current_operation & 0xff, &displacement); 
    // extension
    if (size == 1) {
        source = (int32_t)(int16_t)source;
    }

    uint32_t destination = A((current_operation & 0xe00) >> 9);
    uint32_t tmp;
    
    tmp = destination - source;

    ZERO = source == destination; 
    NEGATIVE = tmp >> 31; // no &1 because extend to 32 bit

    CARRY = ((source & ~destination) | (tmp & ~destination) | (source &
    tmp)) >> 31;

    OVERFLOW = ((~source & destination & ~tmp) | (source & ~destination &
    tmp)) >> 31;

    PC += displacement;

    return 0;
}

/**
* @brief Execute the command cmpi
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
int cmpi(uint16_t current_operation) {
    uint8_t size = (current_operation & 0xc0) >> 6;
    
    uint32_t source, displacement, tmp;
    uint8_t shift;

    uint32_t destination = addressing_mode_source(size, 
        current_operation & 0xff, &displacement); 

    switch (size) {
        case 0x0:
            source = read_16bit(memory + PC + 2) & 0xff;
            shift = 7;
            displacement = 2;
            break;
        case 0x1:
            source = read_16bit(memory + PC + 2);
            shift = 15;
            displacement = 2;
            break;
        case 0x2:
            source = read_32bit(memory + PC + 2);
            shift = 31;
            displacement = 4;
            break;
        default:
            return -1;
    }

    tmp = destination - source;

    ZERO = source == destination; 
    NEGATIVE = (tmp >> shift) & 0x1;

    CARRY = (((source & ~destination) | (tmp & ~destination) | (source &
    tmp)) >> shift) & 0x1;

    OVERFLOW = (((~source & destination & ~tmp) | (source & ~destination &
    tmp)) >> shift) & 0x1;

    PC += displacement;

    return 0;
}

/**
* @brief Execute the command cmpm
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
int cmpm(uint16_t current_operation) {
    uint8_t size = (current_operation & 0xc0) >> 6;
    
    uint32_t displacement = 2;

    uint32_t source, destination, tmp;
    uint8_t shift;

    switch (size) {
        case 0:
            shift = 7;
            source = memory[A(current_operation & 0x7)++]; 
            destination = memory[A((current_operation & 0xe00) >> 9)++];
            break;
        case 1:
            shift = 15;
            source = memory[A(current_operation & 0x7)]; 
            destination = memory[A((current_operation & 0xe00) >> 9)];
            A(current_operation & 0x7) += 2;
            A((current_operation & 0xe00) >> 9) += 2;
            break;
        case 2:
            shift = 31;
            source = memory[A(current_operation & 0x7)]; 
            destination = memory[A((current_operation & 0xe00) >> 9)];
            A(current_operation & 0x7) += 4;
            A((current_operation & 0xe00) >> 9) += 4;
            break;
        default:
            return -1;
    }
   
    tmp = destination - source;

    ZERO = source == destination; 
    NEGATIVE = (tmp >> shift) & 0x1;

    CARRY = (((source & ~destination) | (tmp & ~destination) | (source &
    tmp)) >> shift) & 0x1;

    OVERFLOW = (((~source & destination & ~tmp) | (source & ~destination &
    tmp)) >> shift) & 0x1;

    PC += displacement;

    return 0;
}

/**
 * @brief Set all flag for add.
 *
 * @param source The source value
 * @param destination The destination value
 * @param shift The shift for get the high bit
 */
inline void add_flag(uint32_t source, uint32_t destination,
    uint32_t result, uint8_t shift) {
    ZERO = result == 0; 
    NEGATIVE = (result >> shift) & 0x1;

    CARRY = (((source & destination) | (~result & destination) | (source &
        ~result)) >> shift) & 0x1;
    EXTEND = CARRY; 

    OVERFLOW = (((source & destination & ~result) | (~source & ~destination &
        result)) >> shift) & 0x1;
}

/**
* @brief Execute the command add
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
inline int add(uint16_t current_operation) {
    // info
    uint8_t size = (current_operation & 0xc0) >> 6;
    uint8_t reg = (current_operation & 0xe00) >> 9;
    
    uint32_t displacement = 2;
    uint32_t source, result;
    uint8_t shift;

    // dn + ea -> ea
    if (current_operation & 0x100) {
        // get source
        source = D(reg); 

        // get destination
        uint32_t destination = addressing_mode_source_ro(size,
            current_operation & 0xff); 
        
        // setup and add
        switch (size) {
            case 0x0:
                result = (source + destination) & 0xff; 
                shift = 7;
                break;
            case 0x1:
                result = (source + destination) & 0xffff; 
                shift = 15;
                break;
            case 0x2:
                result = source + destination; 
                shift = 31;
                break;
            default:
                return -1;
        }
        // flag 
        add_flag(source, destination, result, shift);

        // assign
        addressing_mode_destination(size, current_operation & 0xff,
            &displacement, result);
    } else { // ea + dn -> dn
        // get source
        source = addressing_mode_source(size, current_operation & 0xff, 
            &displacement); 
        
        // setup and add
        switch (size) {
            case 0x0:
                result = (source + D(reg)) & 0xff; 
                shift = 7;
                add_flag(source, D(reg), result, shift);
                D(reg) = (D(reg) & 0xffffff00)  | result;
                break;
            case 0x1:
                result = (source + D(reg)) & 0xffff; 
                shift = 15;
                add_flag(source, D(reg), result, shift);
                D(reg) = (D(reg) & 0xffff0000)  | result;
                break;
            case 0x2:
                result = source + D(reg); 
                shift = 31;
                add_flag(source, D(reg), result, shift);
                D(reg) = result;
                break;
            default:
                return -1;
        }
    }

    PC += displacement;
    return 0;
}

/**
* @brief Execute the command adda 
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
inline int adda(uint16_t current_operation) {
    // info
    uint8_t size = current_operation & 0x100 ? 2 : 1;
    
    uint32_t displacement = 2;
    uint32_t source = addressing_mode_source(size, 
        current_operation & 0xff, &displacement); 
    
    // extension
    if (size == 1) {
        // cast word format
        source = (int32_t)(int16_t)source;
    }
    
    // add
    A((current_operation & 0x0e00)>>9) += source;

    PC += displacement;
    return 0;
}

/**
* @brief Execute the command addq
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
inline int addq(uint16_t current_operation) {
    // info
    uint8_t size = (current_operation & 0xc0) >> 6;
    uint32_t source = (current_operation & 0xe00) >> 9;
    if (!source) {
        source = 8;
    }
    
    uint32_t displacement = 2;
    uint32_t result;
    uint8_t shift;

    // get destination
    uint32_t destination = addressing_mode_source_ro(size,
            current_operation & 0xff); 

    // setup and add
    switch (size) {
        case 0x0:
            result = (source + destination) & 0xff; 
            shift = 7;
            break;
        case 0x1:
            result = (source + destination) & 0xffff; 
            shift = 15;
            break;
        case 0x2:
            result = source + destination; 
            shift = 31;
            break;
        default:
            return -1;
    }
    // flag 
    add_flag(source, destination, result, shift);

    // assign
    addressing_mode_destination(size, current_operation & 0xff,
        &displacement, result);

    PC += displacement;
    return 0;
}

/**
* @brief Execute the command addi
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/
inline int addi(uint16_t current_operation) {
    // info
    uint8_t size = (current_operation & 0xc0) >> 6;
    
    uint32_t displacement = 2;
    uint32_t source, destination, result;
    uint8_t shift;

    switch (size) {
        case 0x0:
            PC += 2;
            source = read_16bit(memory + PC);
            shift = 7;
                
            destination = addressing_mode_source_ro(size,
                    current_operation & 0xff); 
            result = (source + destination) & 0xff; 
            break;
        case 0x1:
            PC += 2;
            source = read_16bit(memory + PC);
            shift = 15;

            destination = addressing_mode_source_ro(size,
                    current_operation & 0xff); 
            result = (source + destination) & 0xffff; 
            break;
        case 0x2:
            source = read_32bit(memory + PC + 2);
            PC += 4;
            shift = 31;
            
            destination = addressing_mode_source_ro(size,
                    current_operation & 0xff); 
            result = source + destination; 
            break;
        default:
            return -1;
    }

    // flag 
    add_flag(source, destination, result, shift);

    // assign
    addressing_mode_destination(size, current_operation & 0xff,
        &displacement, result);

    PC += displacement;
    return 0;
}

/**
* @brief Execute the command move
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/

inline int move(uint16_t current_operation) {
    // info
    uint8_t size;
    
    // select size
    switch (current_operation & 0x3000) {
        case 0x1000:
            size = 0;
            break;
        case 0x3000:
            size = 1;
            break;
        default:
            size = 2;
            break;
    }
    
    uint32_t displacement = 2;
    uint32_t source = addressing_mode_source(size, 
        current_operation & 0xff, &displacement); 

    ZERO = source == 0x0;
    CARRY = 0;
    OVERFLOW = 0;
    switch (size) {
        case 0x0:
            NEGATIVE = (source >> 7) & 0x1;
            break;
        case 0x1:
            NEGATIVE = (source >> 15) & 0x1;
            break;
        case 0x2:
            NEGATIVE = source >> 31;
            break;
    }
    
    // format Xn M => M Xn
    uint32_t tmp = (current_operation >> 3 & 0x38)
        | (current_operation >> 9 & 0x7); 

    addressing_mode_destination(size, tmp, &displacement, source);

    PC += displacement;
    return 0;
}

/**
* @brief Execute the command moveq
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/

inline int moveq(uint16_t current_operation) {
    // info
    OVERFLOW = 0;
    CARRY = 0;

    uint32_t displacement = 2;
    uint32_t source = current_operation & 0xff;

    ZERO = source == 0x0;
    NEGATIVE = ((source>>7)&1) == 0x1;

    uint8_t reg = (current_operation & 0xe00)>>9;

    D(reg) = source;
    if(NEGATIVE)
    {
        D(reg)+= 0xffffff00;
    }

    PC += displacement;
    return 0;
}

/**
* @brief Execute the command movea
*
* @param current_operation the current operation
*
* @return -1 => error || other => OK 
*/

inline int movea(uint16_t current_operation) {
    // info
    uint8_t size = 2 ;
    if(current_operation & 0x1000)
        size = 1;
    
    uint32_t displacement = 2;
    uint32_t source = addressing_mode_source(size, 
        current_operation & 0xff, &displacement); 
    
    // add
    uint32_t *destination = &A((current_operation & 0x0e00)>>9);
    
    if (size == 1) {
        // cast word format
        *destination = (source & 0xffff);
    } else { // size == 2
        *destination = source;
    }

    PC += displacement;
    return 0;
}



inline int movem(uint16_t current_operation) {
    // info
    uint8_t size = ((current_operation & 0x40) >>6) +1; // word :1 and long :2
    uint8_t direction = (current_operation & 0x400) >> 10;
    uint16_t mask = read_16bit(memory + PC + 2);
    //printf("mask = 0x%x \n",mask);
    uint32_t displacement = 4;

    if(direction == 1) // A7 --> D0 // (An)+ // (An)+,d1/d2
    {
        for(int i = 0; i<8;i++)
        {
            if((mask & 0x1) == 0x1)
            {
                uint32_t source = addressing_mode_source(size, 
                        current_operation & 0xff, &displacement); 

               addressing_mode_destination(size, 0x00 + i, &displacement, source);
            }
            mask = mask>>1;
        }

        for(int i = 0; i<8;i++)
        {
            if((mask & 0x1) == 0x1)
            {
                uint32_t source = addressing_mode_source(size, 
                        current_operation & 0xff, &displacement); 

                addressing_mode_destination(size, 0x8 + i, &displacement, source);
            }
            mask = mask>>1;
        }
    }
    else // D0 --> A7 // -(An) // d1/d3,-(An)
    {
        for(int i = 0; i<8;i++)
        {
            if((mask & 0x1) == 0x1)
            {
                uint32_t source = addressing_mode_source(size, 
                        0x8 +(0x7 - i), &displacement); 
                addressing_mode_destination(size, current_operation & 0xff, &displacement, source);
            }
            mask = mask>>1;
        }

        for(int i = 0; i<8;i++)
        {
            if((mask & 0x1) == 0x1)
            {
                uint32_t source = addressing_mode_source(size, 
                        0x00 +(0x7 - i), &displacement); 

                addressing_mode_destination(size, current_operation & 0xff, &displacement, source);
            }
            mask = mask>>1;
        }
    }

    PC += displacement;
    return 0;
}



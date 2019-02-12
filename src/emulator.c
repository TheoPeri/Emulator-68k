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
    PC = read_32bit(memory + A7); 
    A7 += 4;

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
    if (!displacement) {
        displacement = read_16bit(memory + PC + 2); 
    }
    
    PC += displacement; 

    return 0;
}

/**
* @brief Execute the command bsr
*
* @return -1 => error || other => OK 
*/
inline int bsr(uint16_t current_operation) {
    uint32_t displacement = current_operation & 0xff; 
    
    // push address
    A7 -= 4;
    
    // bra 
    if (displacement) {
        write_32bit(memory + A7, PC + 2);
    } else {
        write_32bit(memory + A7, PC + 4);
        displacement = read_16bit(memory + PC + 2); 
    }
    
    PC += displacement; 

    return 0;
}

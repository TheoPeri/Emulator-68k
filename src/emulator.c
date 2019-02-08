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

inline int is_ori_to_ccr(uint16_t current_operation) {
    return current_operation == 0x3c;
}

inline int is_ori_to_sr(uint16_t current_operation) {
    return current_operation == 0x7c;
}

inline int is_ori(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x0;
}

inline int is_andi_to_ccr(uint16_t current_operation) {
    return current_operation == 0x23c;
}

inline int is_andi_to_sr(uint16_t current_operation) {
    return current_operation == 0x27c;
}

inline int is_andi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x200;
}

inline int is_subi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x400;
}

inline int is_addi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x600;
}

inline int is_eori_to_ccr(uint16_t current_operation) {
    return current_operation == 0xa3c;
}

inline int is_eori_to_sr(uint16_t current_operation) {
    return current_operation == 0xa7c;
}

inline int is_eori(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0xa00;
}

inline int is_cmpi(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0xc00;
}

inline int is_btst(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x800
        || (0xf1c0 & current_operation) == 0x100;
}

inline int is_bchg(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x840
        || (0xf1c0 & current_operation) == 0x140;
}

inline int is_bclr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x880
        || (0xf1c0 & current_operation) == 0x180;
}

inline int is_bset(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x8c0
        || (0xf1c0 & current_operation) == 0x1c0;
}

inline int is_movep(uint16_t current_operation) {
    return (0xf138 & current_operation) == 0x108;
}

inline int is_movea(uint16_t current_operation) {
    return (0xc1c0 & current_operation) == 0x40;
}

inline int is_move(uint16_t current_operation) {
    return (0xc000 & current_operation) == 0x0;
}

inline int is_move_from_sr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x40c0;
}

inline int is_move_to_ccr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x44c0;
}

inline int is_move_to_sr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x46c0;
}

inline int is_negx(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4000;
}

inline int is_clr(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4200;
}

inline int is_neg(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4400;
}

inline int is_not(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4600;
}

// test
inline int is_ext(uint16_t current_operation) {
    return (0xffbc & current_operation) == 0x4880;
}

inline int is_nbcd(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x4800;
}

inline int is_swap(uint16_t current_operation) {
    return (0xfff8 & current_operation) == 0x4840;
}

inline int is_pea(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x4840;
}

inline int is_illegal(uint16_t current_operation) {
    return current_operation == 0x4afc;
}

inline int is_tas(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x4ac0;
}

inline int is_tst(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x4a00;
}

inline int is_trap(uint16_t current_operation) {
    return (0xfff0 & current_operation) == 0x4e40;
}

inline int is_link(uint16_t current_operation) {
    return (0xfff8 & current_operation) == 0x4e50;
}

inline int is_unlk(uint16_t current_operation) {
    return (0xfff8 & current_operation) == 0x4e58;
}

inline int is_move_usp(uint16_t current_operation) {
    return (0xfff0 & current_operation) == 0x4e60;
}

inline int is_reset(uint16_t current_operation) {
    return current_operation == 0x4e70;
}

inline int is_nop(uint16_t current_operation) {
    return current_operation == 0x4e71;
}

inline int is_stop(uint16_t current_operation) {
    return current_operation == 0x4e72;
}

inline int is_rte(uint16_t current_operation) {
    return current_operation == 0x4e73;
}

inline int is_rts(uint16_t current_operation) {
    return current_operation == 0x4e75;
}

inline int is_trapv(uint16_t current_operation) {
    return current_operation == 0x4e76;
}

inline int is_rtr(uint16_t current_operation) {
    return current_operation == 0x4e77;
}

inline int is_jsr(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x4e80;
}

inline int is_jmp(uint16_t current_operation) {
    return (0xffc0 & current_operation) == 0x4ec0;
}

inline int is_movem(uint16_t current_operation) {
    return (0xfb80 & current_operation) == 0x4880;
}

inline int is_lea(uint16_t current_operation) {
    return (0xf1c0 & current_operation) == 0x41c0;
}

inline int is_chk(uint16_t current_operation) {
    return (0xf1c0 & current_operation) == 0x4180;
}

inline int is_addq(uint16_t current_operation) {
    return (0xf100 & current_operation) == 0x5000;
}


inline int is_subq(uint16_t current_operation) {
    return (0xf100 & current_operation) == 0x5100;
}

inline int is_dbcc(uint16_t current_operation) {
    return (0xf0f8 & current_operation) == 0x50c8;
}

inline int is_scc(uint16_t current_operation) {
    return (0xf0c0 & current_operation) == 0x50c0;
}

inline int is_bra(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x6000;
}

inline int is_bsr(uint16_t current_operation) {
    return (0xff00 & current_operation) == 0x6100;
}

inline int is_bcc(uint16_t current_operation) {
    return (0xf000 & current_operation) == 0x6000;
}

inline int is_moveq(uint16_t current_operation) {
    return (0xf100 & current_operation) == 0x7000;
}

inline int is_divu(uint16_t current_operation) {
    return (0xf1c0 & current_operation) == 0x80c0;
}

inline int is_divs(uint16_t current_operation) {
    return (0xf1c0 & current_operation) == 0x81c0;
}

inline int is_sbcd(uint16_t current_operation) {
    return (0xf1f0 & current_operation) == 0x8100;
}

inline int is_or(uint16_t current_operation) {
    return (0xf000 & current_operation) == 0x8000;
}

inline int is_subx(uint16_t current_operation) {
    return (0xf130 & current_operation) == 0x9100;
}

inline int is_suba(uint16_t current_operation) {
    return (0xf0c0 & current_operation) == 0x90c0;
}

inline int is_sub(uint16_t current_operation) {
    return (0xf000 & current_operation) == 0x9000;
}

inline int is_cmpm(uint16_t current_operation) {
    return (0xf138 & current_operation) == 0xb108;
}

inline int is_eor(uint16_t current_operation) {
    return (0xf100 & current_operation) == 0xb100;
}

inline int is_cmp(uint16_t current_operation) {
    return (0xf100 & current_operation) == 0xb000;
}

inline int is_cmpa(uint16_t current_operation) {
    return (0xf0c0 & current_operation) == 0xb0c0;
}

inline int is_mulu(uint16_t current_operation) {
    return (0xf1c0 & current_operation) == 0xc0c0;
}

inline int is_muls(uint16_t current_operation) {
    return (0xf1c0 & current_operation) == 0xc1c0;
}

inline int is_abcd(uint16_t current_operation) {
    return (0xf1f0 & current_operation) == 0xc100;
}

inline int is_exg(uint16_t current_operation) {
    return (0xf130 & current_operation) == 0xc100;
}

inline int is_and(uint16_t current_operation) {
    return (0xf000 & current_operation) == 0xc000;
}

inline int is_addx(uint16_t current_operation) {
    return (0xf130 & current_operation) == 0xd100;
}

inline int is_adda(uint16_t current_operation) {
    return (0xf0c0 & current_operation) == 0xd0c0;
}

inline int is_add(uint16_t current_operation) {
    return (0xf000 & current_operation) == 0xd000;
}

inline int is_asd(uint16_t current_operation) {
    return (0xfec0 & current_operation) == 0xe0c0
        || (0xf018 & current_operation) == 0xe000;
}

inline int is_lsd(uint16_t current_operation) {
    return (0xfec0 & current_operation) == 0xe2c0
        || (0xf018 & current_operation) == 0xe008;
}

inline int is_roxd(uint16_t current_operation) {
    return (0xfec0 & current_operation) == 0xe4c0
        || (0xf018 & current_operation) == 0xe010;
}

inline int is_rod(uint16_t current_operation) {
    return (0xfec0 & current_operation) == 0xe6c0
        || (0xf018 & current_operation) == 0xe018;
}

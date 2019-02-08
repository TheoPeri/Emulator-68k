#ifndef _EMULATOR_H
#define _EMULATOR_H

int init();
int shutdown();
int next_instruction();

int is_ori_to_ccr(uint16_t current_operation);
int is_ori_to_sr(uint16_t current_operation);
int is_ori(uint16_t current_operation);

int is_andi_to_ccr(uint16_t current_operation);
int is_andi_to_sr(uint16_t current_operation);
int is_andi(uint16_t current_operation);

int is_subi(uint16_t current_operation);
int is_addi(uint16_t current_operation);

int is_eori_to_ccr(uint16_t current_operation);
int is_eori_to_sr(uint16_t current_operation);
int is_eori(uint16_t current_operation);

int is_cmpi(uint16_t current_operation);

int is_btst(uint16_t current_operation);
int is_bchg(uint16_t current_operation);
int is_bclr(uint16_t current_operation);
int is_bset(uint16_t current_operation);

int is_movep(uint16_t current_operation);
int is_movea(uint16_t current_operation);
int is_move(uint16_t current_operation);
int is_move_from_sr(uint16_t current_operation);
int is_move_to_ccr(uint16_t current_operation);
int is_move_to_sr(uint16_t current_operation);

int is_negx(uint16_t current_operation);
int is_clr(uint16_t current_operation);
int is_neg(uint16_t current_operation);
int is_not(uint16_t current_operation);

int is_ext(uint16_t current_operation);
int is_nbcd(uint16_t current_operation);
int is_swap(uint16_t current_operation);
int is_pea(uint16_t current_operation);
int is_illegal(uint16_t current_operation);
int is_tas(uint16_t current_operation);

int is_tst(uint16_t current_operation);

int is_trap(uint16_t current_operation);

int is_link(uint16_t current_operation);
int is_unlk(uint16_t current_operation);

int is_move_usp(uint16_t current_operation);

int is_reset(uint16_t current_operation);

int is_nop(uint16_t current_operation);
int is_stop(uint16_t current_operation);

int is_rte(uint16_t current_operation);
int is_rts(uint16_t current_operation);

int is_trapv(uint16_t current_operation);

int is_rtr(uint16_t current_operation);

int is_jsr(uint16_t current_operation);
int is_jmp(uint16_t current_operation);

int is_movem(uint16_t current_operation);

int is_lea(uint16_t current_operation);

int is_chk(uint16_t current_operation);

int is_addq(uint16_t current_operation);
int is_subq(uint16_t current_operation);

int is_dbcc(uint16_t current_operation);
int is_scc(uint16_t current_operation);

int is_bra(uint16_t current_operation);
int is_bsr(uint16_t current_operation);

int is_bcc(uint16_t current_operation);

int is_moveq(uint16_t current_operation);

int is_divu(uint16_t current_operation);
int is_divs(uint16_t current_operation);

int is_sbcd(uint16_t current_operation);

int is_or(uint16_t current_operation);

int is_subx(uint16_t current_operation);
int is_suba(uint16_t current_operation);
int is_sub(uint16_t current_operation);

int is_cmpm(uint16_t current_operation);

int is_eor(uint16_t current_operation);

int is_cmp(uint16_t current_operation);
int is_cmpa(uint16_t current_operation);

int is_mulu(uint16_t current_operation);
int is_muls(uint16_t current_operation);

int is_abcd(uint16_t current_operation);

int is_exg(uint16_t current_operation);

int is_and(uint16_t current_operation);

int is_addx(uint16_t current_operation);
int is_adda(uint16_t current_operation);
int is_add(uint16_t current_operation);

int is_asd(uint16_t current_operation);
int is_lsd(uint16_t current_operation);

int is_roxd(uint16_t current_operation);
int is_rod(uint16_t current_operation);
#endif

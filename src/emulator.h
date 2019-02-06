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
#endif

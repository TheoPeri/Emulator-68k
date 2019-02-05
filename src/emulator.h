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
#endif

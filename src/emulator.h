#ifndef _EMULATOR_H
#define _EMULATOR_H

int init();
int shutdown();
int next_instruction();

int rts();
int bra(uint16_t current_operation);
int bsr(uint16_t current_operation);

int adda(uint16_t current_operation);
#endif

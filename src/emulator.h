#ifndef _EMULATOR_H
#define _EMULATOR_H

int init();
int shutdown();
int next_instruction();

int rts();
inline int bsr(uint16_t current_operation);
#endif

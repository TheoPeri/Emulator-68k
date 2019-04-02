#ifndef _EMULATOR_H
#define _EMULATOR_H

int init_memory();
int init();
int shutdown_emulator();
int next_instruction();

uint32_t addressing_mode_source
(
    uint8_t size,
    uint8_t value,
    uint32_t *displacement
);

uint32_t addressing_mode_source_ro
(
    uint8_t size,
    uint8_t value
);

void addressing_mode_destination
(
    uint8_t size,
    uint8_t value,
    uint32_t *displacement,
    uint32_t data
);

// return
int rts();

// branch
int bra(uint16_t current_operation);
int bsr(uint16_t current_operation);
int bcc(uint16_t current_operation);

// test
int tst(uint16_t current_operation);

int dbcc(uint16_t current_operation);

// cmp
int cmp(uint16_t current_operation);
int cmpa(uint16_t current_operation);
int cmpi(uint16_t current_operation);
int cmpm(uint16_t current_operation);

// add
int add(uint16_t current_operation);
int adda(uint16_t current_operation);
int addq(uint16_t current_operation);
int addi(uint16_t current_operation);

// move
int move(uint16_t current_operation);
int moveq(uint16_t current_operation);
int movea(uint16_t current_operation);
int movem(uint16_t current_operation);

// lea
int lea(uint16_t current_operation);

// sub
int sub(uint16_t current_operation);
int suba(uint16_t current_operation);
int subq(uint16_t current_operation);
int subi(uint16_t current_operation);


int jsr(uint16_t current_operation);
int clr(uint16_t current_operation);
int lsd(uint16_t current_operation);
#endif

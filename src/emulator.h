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

int rts();

int bra(uint16_t current_operation);
int bsr(uint16_t current_operation);
int bcc(uint16_t current_operation);

int cmp(uint16_t current_operation);
int cmpa(uint16_t current_operation);
int cmpi(uint16_t current_operation);
int cmpm(uint16_t current_operation);

void add_flag
(
    uint32_t source,
    uint32_t destination,
    uint32_t result,
    uint8_t shift
);

int add(uint16_t current_operation);
int adda(uint16_t current_operation);
int addq(uint16_t current_operation);
int addi(uint16_t current_operation);

int move(uint16_t current_operation);
int moveq(uint16_t current_operation);
int movea(uint16_t current_operation);
int movem(uint16_t current_operation);

int sub(uint16_t current_operation);
int suba(uint16_t current_operation);
int subq(uint16_t current_operation);
int subi(uint16_t current_operation);


int jsr(uint16_t current_operation);
int clr(uint16_t current_operation);
int lsd(uint16_t current_operation);
#endif

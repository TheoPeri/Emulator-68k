#ifndef _EMULATOR_H
#define _EMULATOR_H

int init();
int shutdown();
int next_instruction();

uint32_t addressing_mode_source(
    uint8_t size,
    uint8_t value,
    uint32_t *displacement
);

void addressing_mode_destination(
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

int adda(uint16_t current_operation);

int move(uint16_t current_operation);
int moveq(uint16_t current_operation);
int movea(uint16_t current_operation);

#endif

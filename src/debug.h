#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdint.h>

char* mystrcat(char* a, char* b);

void uint32_tostring(char *buffer, uint32_t value);
void memory_tostring(char *buffer, char *value, unsigned size);

void togglebreakpoint(uint32_t address);

void show_current_instruction(char** addrs, char** opcode, char** operands);
void pretty_print_instruction(char** addrs, char** opcode, char** operands);

#endif

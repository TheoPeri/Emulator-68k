#ifndef _DEBUG_H
#define _DEBUG_H
#include <stdint.h>

void show_current_instruction(char** addrs, char** opcode, char** operands);
void pretty_print_instruction(char** addrs, char** opcode, char** operands);
void uint32_tostring(char *buffer, uint32_t value);

#endif

#ifndef _LOADER_H
#define _LOADER_H
#include <stdint.h>

int copy_raw_data(char *s, uint8_t size, uint32_t address);
int load_line(char *s);

#endif

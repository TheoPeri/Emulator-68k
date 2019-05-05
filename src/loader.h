#ifndef _LOADER_H
#define _LOADER_H

#include <stdint.h>

uint8_t compute_checksum(char *s, unsigned size);
int copy_raw_data(char *s, uint8_t size, uint32_t address);

int load_line(char *s);
int load_file(char *name);

#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "emulator.h"
#include "loader.h"

extern uint8_t *memory;

int load(char *s) {
    size_t offset, tmp, size;
    unsigned type, checksum;
    uint32_t address;

    offset = sscanf(s, "S%1d", &type);

    switch (type) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            tmp = sscanf(s + offset, "%2lx", &size); 
            offset += tmp;

            tmp = sscanf(s + offset, "%6x", &address);
            offset += tmp;
            
            memcpy(memory + address, s + offset, 2 * (size-3-1) * sizeof(char));

            offset += 2 * (size - 3 - 1) * sizeof(char);
            tmp = sscanf(s + offset, "%2x", &checksum);
            offset += tmp;

            // checksum
            break;
        case 3:
            break;
        case 5:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        default:
            return -1; 
    }

    return 0;
}

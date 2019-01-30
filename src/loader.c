#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "emulator.h"
#include "loader.h"

extern uint8_t *memory;

/**
 * @brief Copy every byte in the memory.
 *
 * @param s The data string.
 * @param size The size of the data.
 * @param address The relative address where paste the data in memory.
 *
 * @return -1 => error || other => the sum of every byte paste
 */
int copy_raw_data(char *s, uint8_t size, uint32_t address) {
    uint8_t res = 0;

    for (unsigned i = 0; i < size; ++i) {
        if (sscanf(s, "%2hhx", memory + address + i) == EOF) {
            fprintf(stderr, "Error while parsing: %s.\n", strerror(errno));  
            return -1;
        }

        s += 2;
        res += memory[address + i];
    }

    return (int)res;
}

/**
 * @brief Load on line in the memory (S-Record).
 *
 * @param s The string wich contain the data.
 *
 * @return -1 => error | other => OK
 */
int load_line(char *s) {
    unsigned offset, type, size, checksum, address;
    int tmp;

    if (sscanf(s, "S%1d", &type) == EOF) {
        goto error;
    }

    switch (type) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            if (sscanf(s + 2, "%2x", &size) == EOF ||
                sscanf(s + 4, "%6x", &address) == EOF) {
                goto error;
            }

            // copy data in the memory             
            if ((tmp = copy_raw_data(s + 10, size - 4, address)) == -1) {
                return -1;
            }
            
            offset = 10 + 2 * (size - 1);
            if ((sscanf(s + offset, "%2x", &checksum)) == EOF) {
                goto error;
            }

            // checksum
            if (checksum != (~(size + tmp + address) & 0xFF)) {
                fprintf(stderr, "WARNING: checksum value incorrect expect: %u,"
                    "received: %u", checksum, ~(size + tmp + address) & 0xFF);
            }

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
            goto error;
    }

    return 0;

    error:
        fprintf(stderr, "Error while parsing: %s.\n", strerror(errno));  
        return -1;
}

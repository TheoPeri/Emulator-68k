#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "emulator.h"
#include "loader.h"

extern uint8_t *memory;
extern uint32_t PC;

/**
 * @brief Basic implementation of checksum.
 *
 * @param s The string to hash.
 * @param size The size of the string.
 *
 * @return -1 => error || other => the checksum
 */
uint8_t compute_checksum(char *s, unsigned size) {
    uint32_t res, tmp;
    res = 0;

    for (unsigned i = 0; i < size; ++i) {
        if (sscanf(s, "%2x", &tmp) == EOF) {
            fprintf(stderr, "Error while parsing: %s.\n", strerror(errno));  
            return 0;
        }
        res += tmp;
        s += 2;
    }
    
    return (uint8_t)~res;
}

/**
 * @brief Copy every byte in the memory.
 *
 * @param s The data string.
 * @param size The size of the data.
 * @param address The relative address where paste the data in memory.
 *
 * @return -1 => error || other => OK
 */
int copy_raw_data(char *s, uint8_t size, uint32_t address) {
    for (unsigned i = 0; i < size; ++i) {
        if (sscanf(s, "%2hhx", memory + address + i) == EOF) {
            fprintf(stderr, "Error while parsing: %s.\n", strerror(errno));  
            return -1;
        }

        s += 2;
    }

    return 0;
}

/**
 * @brief Load on line in the memory (S-Record).
 *
 * @param s The string wich contain the data.
 *
 * @return -1 => error | other => OK
 */
int load_line(char *s) {
    unsigned type, size, size_address, address;
    uint8_t checksum;
    int tmp = 0;

    if (sscanf(s++, "S%1d", &type) == EOF || sscanf(++s, "%2x", &size) == EOF) {
        goto error;
    }
    
    if (type == 0 || type == 5) {
        // informative S
        if (sscanf(s + 2, "%4x", &address) == EOF) {
            goto error;
        }

        if (type == 0) {
            printf("Find header header content: ");
            write(1, s + 6, size - 3);
            printf("\n");
        } else {
            printf("The count of S1, S2, and S3 records previously transmitted"
                    ": %u\n", address);
        }
    } else if (type == 1 || type == 2 || type == 3) {
        // data S
        switch (type) {
            case 1:
                size_address = 4;
                if (sscanf(s + 2, "%4x", &address) == EOF) {
                    goto error;
                }
                break;
            case 2:
                size_address = 6;
                if (sscanf(s + 2, "%6x", &address) == EOF) {
                    goto error;
                }
                break;
            case 3:
                size_address = 8;
                if (sscanf(s + 2, "%8x", &address) == EOF) {
                    goto error;
                }
                break;
        }
        
        // copy data in the memory             
        if ((tmp = copy_raw_data(s + 2 + size_address,
            size - 1 - size_address/2, address)) == -1) {
            return -1;
        }
    } else if (type == 7 || type == 8 || type == 9) {
        // start execution S
        switch (type) {
            case 9:
                if (sscanf(s + 2, "%4x", &address) == EOF) {
                    goto error;
                }
                break;
            case 8:
                if (sscanf(s + 2, "%6x", &address) == EOF) {
                    goto error;
                }
                break;
            case 7:
                if (sscanf(s + 2, "%8x", &address) == EOF) {
                    goto error;
                }
                break;

            // copy the data in the PC
            PC = address;
        }
    } else {
        fprintf(stderr, "Unknow S type => S%u", type);
        return -1;
    }

    if (sscanf(s + 2 * size, "%2hhx", &checksum) == EOF) {
        goto error;
    }

    // checksum
    if (checksum != compute_checksum(s, size)) {
        fprintf(stderr, "WARNING: checksum value incorrect expect: 0x%hhx,"
                "received: 0x%hhx\n", checksum, compute_checksum(s, size));
    }

    return 0;

    error:
        fprintf(stderr, "Error while parsing: %s.\n", strerror(errno));  
        return -1;
}

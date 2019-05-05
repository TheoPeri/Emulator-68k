#define _GNU_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "loader.h"
#include "memory.h"

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
            warnx("Error while parsing: %s\n", strerror(errno));
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
            warnx("Error while parsing => %s\n", strerror(errno));
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
            printf("Find header content => ");
            char c;

            for (unsigned i = 0; i < size-3; ++i) {
                sscanf(s + 6 + 2 * i, "%2hhx", &c);
                printf("%c", c);
            }

            printf("\n");
        } else {
            printf("The count of S1, S2, and S3 records previously transmitted"
                    " => %u\n", address);
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
        }

        // copy the data in the PC
        if ((PC = address)) {
            printf("Set the PC address at 0x%06x\n", address);
        } else {
            printf("No information in the .hex file for the PC\n");
        }
    } else {
        warnx("Unknow S type => S%u\n", type);
        return -1;
    }

    if (sscanf(s + 2 * size, "%2hhx", &checksum) == EOF) {
        goto error;
    }

    // checksum
    if (checksum != compute_checksum(s, size)) {
        warnx("WARNING: checksum value incorrect expect: 0x%hhx,"
            "received => 0x%hhx\n", checksum, compute_checksum(s, size));
    }

    return 0;

    error:
        warnx("Error while parsing => %s\n", strerror(errno));
        return -1;
}

/**
 * @brief Load the file in the memory.
 *
 * @param name_file The name of the file to load.
 *
 * @return -1 => error | other => OK
 */
int load_file(char *name_file) {
    FILE *fp;
    char *line = NULL;
    size_t i = 0;
    size_t len = 0;
    ssize_t read;
    fp = fopen(name_file, "r");

    if (fp == NULL) {
        warnx("FAIL OPEN THE FILE!!!.\n");
        return -1;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("\nLoad the line number %li => %s", i, line);
        load_line(line);
        ++i;
    }

    fclose(fp);
    if (line) {
        free(line);
    }

    return 0;
}

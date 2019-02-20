#define _GNU_SOURCE

#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <capstone/capstone.h>

#include "memory.h"

extern uint8_t *memory;

#define code_size 64

char *mystrcat(char *s1, char *s2) {
    size_t len = strlen(s1) + strlen(s2); 

    if ((s1 = realloc(s1, (len + 1) * sizeof(char))) == NULL) {
        return NULL;
    }

    char *tmp = s1 + strlen(s1); 

    while (*s2 != '\0') {
        *tmp++ = *s2++;
    }

    *tmp = '\0';

    return s1;
}

/**
 * @brief Print the current => +30 instructions
 *
 * @return -1 => error || other => size of the first instruction 
 */
char *show_current_instruction() {
    csh handle;
	cs_insn *insn;
	size_t count;
	char *tmp = NULL;
	char *res = calloc(1, sizeof(char)); // init ""

	if (cs_open(CS_ARCH_M68K, CS_MODE_BIG_ENDIAN, &handle) != CS_ERR_OK) {
		warnx("ERROR: Failed to open the given code!\n");
		return NULL;
    }

	count = cs_disasm(handle, memory + PC, code_size, PC, 0, &insn);
    
    res = mystrcat(res, "-->");
	if (count > 0) {
		size_t j;
        if (count >= 1) {
            asprintf(&tmp, "\t0x%06lx\t%-20s%s\n", insn[0].address,
                insn[0].mnemonic, insn[0].op_str);
            
            res = mystrcat(res, tmp);
            free(tmp);
		}

		for (j = 1; j < count && j < 30; j++) {
            asprintf(&tmp, "\t0x%06lx\t%-20s%s\n", insn[j].address,
                insn[j].mnemonic, insn[j].op_str);
            
            res = mystrcat(res, tmp);
            free(tmp);
		}

		cs_free(insn, count);
	} else {
		warnx("ERROR: Failed to disassemble given code!\n");
	    cs_close(&handle);
        return NULL;
    }

	cs_close(&handle);
    return res;
}

/**
 * @brief Pretty print of the current => +30 instuctions
 *
 * @return -1 => error || other => size of the first instruction
 */
char *pretty_print_instruction() {
    char *tmp;
    char *res = calloc(1, sizeof(char));
    
    asprintf(&tmp, "=========================================================="
    "====\n========[Address]=======[Opcode]==============[Operande]======\n");
    res = mystrcat(res, tmp);
    free(tmp);

    tmp = show_current_instruction();
    res = mystrcat(res, tmp);
    free(tmp);

    asprintf(&tmp, "=========================================================="
    "====\n");
    res = mystrcat(res, tmp);
    free(tmp);

    return res;
}

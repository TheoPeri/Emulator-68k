#define _GNU_SOURCE

#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <capstone/capstone.h>

#include "memory.h"
#include "dictionary.h"

extern uint8_t *memory;

// capstone macro
#define code_size 512
#define NB_INSTRUCTION 34

#define GREEN " <span color='green'>%s</span>\n"

/**
 * @brief A basic custom str cat
 *
 * @param s1 First string
 * @param s2 Second string
 *
 * @return The fusion of the 2 string.
 */
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
 * @brief Converts uint32 to str
 *
 * @param buffer The output buffer
 * @param value The value to convert
 * @param size The size of the value to convert
 *
 */
void uint32_tostring(char *buffer, uint32_t value) {
	unsigned i;
	char *c_value = (char *)&value;

	for (i = 0; i < 4; ++i)
	{
        if (c_value[i] < 0x20 || c_value[i] == 0x7f) {
            buffer[3 - i] = '.';
        } else {
            buffer[3 - i] = c_value[i];
        }
	}

	buffer[i] = '\0';
}

/**
 * @brief Converts uint32 to str (no xml)
 *
 * @param buffer The output buffer
 * @param value The value to convert
 * @param size The size of the value to convert
 *
 */
void memory_tostring(char *buffer, char *value, unsigned size) {
	unsigned i;

	for (i = 0; i < size; ++i)
	{
        if (value[i] < 0x20 || value[i] == 0x7f) {
            buffer[i] = '.';
        } else {
            buffer[i] = value[i];
        }
	}

	buffer[i] = '\0';
}

/**
 * @brief Toogle a breakpoint on the address.
 *
 * @param address The address of the breakpoint
 */
void togglebreakpoint(uint32_t address) {
	if(!dict_get(break_points, address)) {
		dict_insert(break_points, address, 1);
    } else {
		dict_remove(break_points, address);
    }
}

/**
 * @brief Print the current => +30 instructions
 *
 * @return -1 => error || other => size of the first instruction
 */
void show_current_instruction(char** addrs, char** opcodes, char** operandes) {
    csh handle;
	cs_insn *insn;
	cs_insn *binsn;
	size_t count;
	char *tmp = NULL;

	if (cs_open(CS_ARCH_M68K, CS_MODE_BIG_ENDIAN, &handle) != CS_ERR_OK) {
		warnx("ERROR: Failed to open the given code!\n");
		return;
    }

	count = cs_disasm(handle, memory + PC, code_size, PC, 0, &insn);

	size_t back_count = cs_disasm(handle, memory, PC, 0, 0, &binsn);

	if(back_count > 0)
	{
		size_t j;
		*addrs = mystrcat(*addrs, "<span color='lightgray'>");
		*opcodes = mystrcat(*opcodes, "<span color='lightgray'>");
		*operandes = mystrcat(*operandes, "<span color='lightgray'>");
		size_t c = back_count < 8 ? back_count : 8;
		for(j = 0; j < c; j++)
		{
			asprintf(&tmp,dict_get(break_points, binsn[back_count - c + j].address) ? " * $%06lX\n" :"   $%06lX\n", binsn[back_count - c + j].address);
			*addrs = mystrcat(*addrs, tmp);
			free(tmp);

			asprintf(&tmp, " %s\n", binsn[back_count - c + j].mnemonic);
			*opcodes = mystrcat(*opcodes, tmp);
			free(tmp);

			asprintf(&tmp, " %s\n", binsn[back_count - c + j].op_str);
			*operandes = mystrcat(*operandes, tmp);
			free(tmp);
		}
		*addrs = mystrcat(*addrs, "</span>");
		*opcodes = mystrcat(*opcodes, "</span>");
		*operandes = mystrcat(*operandes, "</span>");
		cs_free(binsn, back_count);
	}

	if (count > 0) {
		size_t j;
		for (j = 0; j < count && j < NB_INSTRUCTION; j++) {
			asprintf(&tmp, j == 0 ?
				(dict_get(break_points, insn[j].address) ? "*> <span color='green'>$%06lX</span>\n" :
														 "-> <span color='green'>$%06lX</span>\n") :
				(dict_get(break_points, insn[j].address) ? " * $%06lX\n" :"   $%06lX\n"), insn[j].address);

			*addrs = mystrcat(*addrs, tmp);
			free(tmp);

			asprintf(&tmp, j == 0 ? GREEN :" %s\n", insn[j].mnemonic);
			*opcodes = mystrcat(*opcodes, tmp);
			free(tmp);

			asprintf(&tmp, j == 0 ? GREEN : " %s\n", insn[j].op_str);
			*operandes = mystrcat(*operandes, tmp);
			free(tmp);
		}

		cs_free(insn, count);
	} else {
		warnx("ERROR: Failed to disassemble given code!\n");
	    cs_close(&handle);
        return;
    }

	cs_close(&handle);
    return;
}

/**
 * @brief Pretty print of the current => +30 instuctions
 *
 * @return -1 => error || other => size of the first instruction
 */
void pretty_print_instruction(char** addresses, char** opcode, char** operande) {
    char* addrs = calloc(1, sizeof(char));
	char* op = calloc(1, sizeof(char));
	char* o = calloc(1, sizeof(char));

	addrs = mystrcat(addrs, "<span font_family='Monospace'>"
							"<span color='blue'>[Address]</span>\n");

	op = mystrcat(op, "<span font_family='Monospace'>"
						"<span color='blue'>[Opcode]</span>\n");

	o = mystrcat(o, "<span font_family='Monospace'>"
					"<span color='blue'>[Operande]</span>\n");

    show_current_instruction(&addrs, &op, &o);

    addrs = mystrcat(addrs, "</span>");
	op = mystrcat(op, "</span>");
	o = mystrcat(o, "</span>");

	*addresses = addrs;
	*opcode = op;
	*operande = o;
}

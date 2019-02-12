#include <stdio.h>
#include <err.h>
#include <capstone/capstone.h>

#include "memory.h"

#define code_size 100

extern uint32_t PC;
extern uint8_t *memory;

/**
 * @brief Print the current => +30 instructions
 *
 * @return -1 => error || other => size of the first instruction 
 */
int show_current_instruction() {
    csh handle;
	cs_insn *insn;
	size_t count;

    size_t size = 0;

	if (cs_open(CS_ARCH_M68K, CS_MODE_BIG_ENDIAN, &handle) != CS_ERR_OK) {
		warnx("ERROR: Failed to open the given code!\n");
		return -1;
    }

	count = cs_disasm(handle, memory + PC, code_size, PC, 0, &insn);
    
    printf("-->");
	if (count > 0) {
		size_t j;
        if (count >= 1) {
            size = insn[1].size;

            printf("  0x%06lx:       %s\t\t%s\n", insn[0].address,
                    insn[0].mnemonic, insn[0].op_str);
		}

		for (j = 1; j < count && j < 30; j++) {
			printf("     0x%06lx:       %s\t\t%s\n", insn[j].address, 
                    insn[j].mnemonic, insn[j].op_str);
		}

		cs_free(insn, count);
	} else {
		warnx("ERROR: Failed to disassemble given code!\n");
	    cs_close(&handle);
        return -1;
    }

	cs_close(&handle);
    return size;
}

/**
 * @brief Pretty print of the current => +30 instuctions
 *
 * @return -1 => error || other => size of the first instruction
 */
int pretty_print_instruction() {
    int tmp;

    printf("==============================================================\n");
    printf("=====[Address]======[Opcode]================[Operande]========\n");
    tmp = show_current_instruction();
    printf("==============================================================\n");

    return tmp;
}

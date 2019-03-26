#ifndef _MEMORY_H
#define _MEMORY_H

// easy memory management macro

#define MEMORY() memory

#define ALTERNATIVES_MEMORY(tmp) (tmp)

/**
 * @brief Remplaced the selected memory (nothing => memory | other => other)
 */
#define SELECTED_MEMORY(...) (__VA_OPT__(ALTERNATIVES_)##MEMORY(__VA_OPT__((__VA_ARGS__))))

#define read_8bit_memory(addr, ...) ((SELECTED_MEMORY(__VA_ARGS__))[(addr)])
#define read_16bit_memory(addr, ...) (read_16bit((SELECTED_MEMORY(__VA_ARGS__)) + (addr)))
#define read_32bit_memory(addr, ...) (read_32bit((SELECTED_MEMORY(__VA_ARGS__)) + (addr)))

/**
 * @brief Simplify basic memory access
 *
 * @param output the output var
 * @param d the displacement
 * @param size the size of the memory to select
 */
#define SIMPLE_READ_MEMORY_SIZE(output, d, size) ({\
switch ((size)) { \
    case 0x0: \
        (output) = memory[(d)]; \
        break; \
    case 0x1:\
        (output) = read_16bit(memory + (d));\
        break;\
    case 0x2:\
        (output) = read_32bit(memory + (d));\
        break; \
}})


/**
 * @brief Simplify pre op memory access
 *
 * @param output the output var
 * @param d the displacement
 * @param size the size of the memory to select
 * @param to_mod the var to modified
 * @param nb_b value used to postincrement for b size
 * @param nb_w value used to postincrement for w size
 * @param nb_l value used to postincrement for l size
 */
#define POST_INC_READ_MEMORY_SIZE(output, d, size, to_mod, nb_b, nb_w, nb_l) ({\
switch ((size)) { \
    case 0x0: \
        (output) = memory[(d)]; \
        (to_mod) += (nb_b);\
        break; \
    case 0x1:\
        (output) = read_16bit(memory + (d));\
        (to_mod) += (nb_w);\
        break;\
    case 0x2:\
        (output) = read_32bit(memory + d); \
        (to_mod) += (nb_l);\
        break; \
}})

/**
 * @brief Simplify post op memory access
 *
 * @param output the output var
 * @param d the displacement
 * @param size the size of the memory to select
 * @param to_mod the var to modified
 * @param nb_b value used to decrement for b size
 * @param nb_w value used to decrement for w size
 * @param nb_l value used to decrement for l size
 */
#define PRE_DEC_READ_MEMORY_SIZE(output, d, size, to_mod, nb_b, nb_w, nb_l) ({\
switch ((size)) { \
    case 0x0: \
        (to_mod) -= (nb_b);\
        (output) = memory[(d)]; \
        break; \
    case 0x1:\
        (to_mod) -= (nb_w);\
        (output) = read_16bit(memory + (d));\
        break;\
    case 0x2:\
        (to_mod) -= (nb_l);\
        (output) = read_32bit(memory + d); \
        break; \
}})

// memory
extern uint8_t *memory;

// registers
extern uint32_t PC;

// status registers
extern int8_t status_registers[6];

// data and address registers
extern uint32_t registers[17];

#define CARRY                           status_registers[0]
#define OVERFLOW                        status_registers[1]
#define ZERO                            status_registers[2]
#define NEGATIVE                        status_registers[3]
#define EXTEND                          status_registers[4]
#define SUPERVISOR_STATE                status_registers[5]

#define D(i) registers[i]

#define A(i) (registers + 8)[i]

uint32_t get_stack_ptr();

uint32_t read_16bit(uint8_t *address);

uint32_t read_32bit(uint8_t *address);

void write_8bit(uint8_t *address, uint32_t value);

void write_16bit(uint8_t *address, uint32_t value);

void write_32bit(uint8_t*address, uint32_t value);
#endif

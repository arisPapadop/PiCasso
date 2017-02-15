#include "emulator_util.h"

// Print method for emulate, used when the program stops running to output the
// final state of the ARM11. 
void print_registers(State *state) {
    printf("Registers:\n");

    for (int i = 0; i < GP_REGISTERS; i++) {
        uint32_t reg_i = (*state).registers.general_purpose[i];
        printf("$%-3d: %10d (0x%08x)\n", i, reg_i, reg_i);
    }
    printf("%-4s: %10d (0x%08x)\n", "PC",
           (*state).registers.PC, (*state).registers.PC);

    uint32_t cspr_value = ((*state).registers.CPSR.N << 31) |
                          ((*state).registers.CPSR.Z << 30) |
                          ((*state).registers.CPSR.C << 29) |
                          ((*state).registers.CPSR.V << 28);

    printf("%-4s: %10d (0x%08x)\n", "CPSR", cspr_value, cspr_value);

    printf("Non-zero memory:\n");
    for (int i = 0; i < CAPACITY; i += WORD_SIZE) {
        int value = word_to_print_build((*state).memory + i);
        if (value != 0) {
            printf("0x%08x: 0x%08x\n", i, value);
        }
    }
}

// The memory is byte addressable but all words that need to be printed consist
// of 4 bytes, so bytes need to be combined in groups of 4 (WORD_SIZE). The 
// function also changes the endianness, to match the desired output.
uint32_t word_to_print_build(uint8_t *byte) {
    uint32_t value = 0;
    for (int i = WORD_SIZE - 1; i >= 0; i--) {
        value |= (*byte << (sizeof(byte) * i));
        byte++;
    }
    return value;
}

void store_word_in_memory(uint32_t data, uint8_t *byte) {
    for (int i = WORD_SIZE-1; i >= 0 ; --i) {
        uint8_t current_byte = byte_from_word(data, i);
        byte[WORD_SIZE - i - 1] = current_byte;
    }
}
// This function is similar to word_to_print_build but it keeps the endianness
// unchanged.
uint32_t word_build(uint8_t *byte) {
    uint32_t value = 0;
    for (int i = 0; i < WORD_SIZE; i++) {
        uint32_t tmp = (uint32_t)(*byte);
        value |= (tmp << (sizeof(byte) * i));
        byte++;
    }
    return value;
}

// Returns the sign extended 2's complement representation of a 24bit number.
int signExtend(int number) {
    int value = ((0 << 31) | number);
    int mask  = 1 << 25;
    if (mask & number) {
        value += 0xFC000000;
    }
    return value;
}

bool gpio_access(uint32_t mem_address) {
    return (mem_address == GPIO_ZERO ||
            mem_address == GPIO_TEN ||
            mem_address == GPIO_TWENTY);
}

bool memory_in_bounds(uint32_t mem_address) {
  bool result = mem_address < CAPACITY;

    switch(mem_address) {
        case GPIO_ZERO:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            break;
        case GPIO_TEN:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            break;
        case GPIO_TWENTY:
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            break;
        case GPIO_ON:
            printf("PIN ON\n");
            break;
        case GPIO_OFF:
            printf("PIN OFF\n");
            break;
        default:
            if (!result) {
                printf("Error: Out of bounds memory access"
                               " at address 0x%08x\n", mem_address);
            }
            break;
    }

  return result;
}

size_t bytes_count(FILE *file) {
    fseek(file, 0, SEEK_END);
    long bytes = ftell(file);

    // Reset stream position and return.
    fseek(file, 0, SEEK_SET);
    return (size_t)bytes;
}
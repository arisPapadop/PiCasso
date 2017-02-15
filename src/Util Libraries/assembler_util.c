#include "assembler_util.h"

// Prints the binary representation of 32bit integers in standard output.
void print_binary(uint32_t input) {

    // Start with a mask that is 1 at the MSB and then 0 and then 
    // shift the input to the right.
    uint32_t mask = 1 << 31;

    for (int i = 0; i < 32; ++i) {
        // Leave a blank space every byte.
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }

        if ((input & mask) == 0) {
            printf("0");
        } else {
            printf("1");
        }

        input <<= 1;
    }
    printf("\n");
}

uint32_t number_build(char *str) {
    if (str[0] == '[') {
        str++;
    }
    if (str[0] == 'r') {
        str++;
    }
    if (str[0] == '=' || str[0] == '#') {
        str++;
        int sign = 1;
        if (str[0] == '-') {
            str++;
            sign = -1;
        }

        if (is_hex(str)) {
            return (sign * build_with_base(str, 16));
        } else {
            return (sign * build_with_base(str, 10));
        }
    } else if (isdigit(str[0])) {
        if (is_hex(str)) {
            return build_with_base(str, 16);
        } else {
            return build_with_base(str, 10);
        }
    } else {
        perror("Error: Invalid number format.");
        return -1;
    }
}


Token tokenize(char* str) {
    StringArray   args;
    Token       result;
    char*     mnemonic;


    const char seps[4] = " ,\n";

    char *token   = strtok(str, seps);
    args.elements = (char **)malloc(5 * sizeof(void *));

    uint32_t i = 0;
    while (token != NULL) {
        if (i == 0) {
            mnemonic = token;
        } else {
            args.elements[i - 1] = token;
        }
        token = strtok(NULL, seps);
        i++;
    }
    //args.elements = (char **)realloc(args.elements, i);
    args.size     = i;

    result.mnemonic  = mnemonic;
    result.arguments = args;

    return result;
}

// Checks if line represents label, assuming that no in-line labels exist, i.e.
// it assumes  that all labels are in the form: <text><:><\n>.
bool is_label(char *line) {
  return (line[strlen(line) - LABEL_CONSTANT] == LABEL_SEPARATOR);
}

// Returns true iff the given string is a register, assuming that all registers
// start with an r.
bool is_register(char *str) {
    // Check the case that the given string is part of a SDT instruction, e.g a
    // str.
    if (str[0] == '[') {
        str++;
    }
    return (str[0] == 'r');
}

// TEMPORAL 
int get_length(char *str) {
    int size = 0;
    while (*str != END_OF_STRING) {
        size++;
        str++;
    }
    return size;
}

// Returns true iff the given string is the hex representatin of a number, so 
// only if it has the form 0x_____
bool is_hex(char *str) {
    if (str[1] == 'x') {
        str += 2;
        return true;
    }
    return false;
}

uint32_t build_with_base(char *exp, uint16_t base) {
    return (uint32_t)strtol(exp, NULL, base);
}

uint8_t register_number_build(char *reg) {
    return (uint8_t)number_build(reg);
}

// Returns the operand in an instruction.
uint32_t get_operand(char *op, uint8_t *imm) {
    if (op[0] == '#') {
        // Is an expression
        *imm = 1;
        return number_build(op);
    } else {
        // Must be a register
        *imm = 0;
        return register_number_build(op);
    }
}

uint8_t get_shift_type(char *shift) {
    switch (shift[0]) {
        case 'l':
            if (shift[2] == 'l') {
                return LSL;
            } else {
                return LSR;
            }
        case 'a':
            return ASR;
        case 'r':
            return ROR;
        default:
            perror("Error: Not a valid shift type");
            return 0;
    }
}

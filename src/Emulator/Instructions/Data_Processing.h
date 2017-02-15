#include <limits.h>

uint8_t test_addition_overflow(uint32_t a, uint32_t b);

void execute_data_processing(State *state, Data_Processing *instruction) {
    uint8_t Rd = (*instruction).Rd;
    uint8_t Rn = (*instruction).Rn;

    Shift_Struct shift_struct;

    if ((*instruction).I) {
        // Operand2 is an immediate constant
        // Imm = bits 0-7, extended to 32 bits
        shift_struct = shift_immed((*instruction).operand2);
    } else {
        // Operand2 is a shifted register
        // shift_op2 returns a structure, need .shift
        shift_struct = shift_op2(state, (*instruction).operand2);
    }

    uint32_t operand2 = shift_struct.shift;

    uint32_t *GP_registers = (*state).registers.general_purpose;
    uint32_t result;
    uint8_t C = shift_struct.C;
    //By default C is set to return of shift_* functions

    switch ((*instruction).opcode) {
        case AND:
            result = GP_registers[Rn] & operand2;
            GP_registers[Rd] = result;
            break;
        case EOR:
            result = GP_registers[Rn] ^ operand2;
            GP_registers[Rd] = result;
            break;
        case SUB:
	    // If - produces borrow, C = 0
	        if (operand2 <= GP_registers[Rn]) {
                C = 1;
            } else {
                C = 0;
            }
            result = GP_registers[Rn] - operand2;
            GP_registers[Rd] = result;
            break;
        case RSB:
	    // If - produces borrow, C = 1
	        if (GP_registers[Rn] <= operand2) {
               C = 1;
            } else {
                C = 0;
            }
            result = operand2 - GP_registers[Rn];
            GP_registers[Rd] = result;
            break;
        case ADD:
	    // If + produces carry, C = 1
	        if (test_addition_overflow(operand2, GP_registers[Rn]) == 0) {
                C = 0;
            } else {
                C = 1;
            }
            result = operand2 + GP_registers[Rn];
            GP_registers[Rd] = result;
            break;
        case TST:
            result = GP_registers[Rn] & operand2;
            break;
        case TEQ:
            result = GP_registers[Rn] ^ operand2;
            break;
        case CMP:
	    // If - produces borrow, C = 1
	        if (operand2 <= GP_registers[Rn]) {
                C = 1;
            } else {
                C = 0;
            }
            result = GP_registers[Rn] - operand2;
            break;
        case ORR:
            result = operand2 | GP_registers[Rn];
            GP_registers[Rd] = result;
            break;
        case MOV:
            result = operand2;
            GP_registers[Rd] = result;
            break;
    }
    if ((*instruction).S) {
        (*state).registers.CPSR.N = get_bit(result, 31);
        (*state).registers.CPSR.Z = result == 0;
        (*state).registers.CPSR.C = C;
    }
}

uint8_t test_addition_overflow(uint32_t a, uint32_t b) {
    return (INT32_MAX - b < a || INT32_MAX - a < b);
}

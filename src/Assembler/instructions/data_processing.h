Instruction encode_data_processing(uint8_t opcode, uint8_t S,
                                   char *Rd, char *Rn, char *operand2) {
    Instruction instruction;
    instruction.cond = AL; // Always
    instruction.type = DATA_PROCESSING;

    instruction.data_processing.opcode   = opcode;
    instruction.data_processing.S        = S;
    instruction.data_processing.Rd       = register_number_build(Rd);
    instruction.data_processing.Rn       = register_number_build(Rn);

    // get_operand also sets I bit
    uint32_t operand2_value
            = get_operand(operand2, &(instruction.data_processing.I));

    check_rotations(operand2_value, &(instruction.data_processing.operand2));

    return instruction;
}

Instruction encode_optional(char *Rd, char *Rn,
                            StringArray arguments, uint8_t opcode);

Instruction encode_binop(StringArray arguments, uint8_t opcode) {
    char *Rd       = arguments.elements[0];
    char *Rn       = arguments.elements[1];
    char *operand2 = arguments.elements[2];

    if (arguments.elements[3] != NULL) {
        return encode_optional(Rd, Rn, arguments, opcode);
    }

    // S = 0;
    return encode_data_processing(opcode, 0, Rd, Rn, operand2);
}

Instruction encode_test(StringArray arguments, uint8_t opcode) {
    char *Rn       = arguments.elements[0];
    char *operand2 = arguments.elements[1];

    // S = 1; Rd = 0;
    return encode_data_processing(opcode, 1, "r0", Rn, operand2);
}

/* BINARY OPERATIONS */

Instruction encode_add(StringArray arguments) {
    return encode_binop(arguments, ADD);
}

Instruction encode_sub(StringArray arguments) {
    return encode_binop(arguments, SUB);
}

Instruction encode_rsb(StringArray arguments) {
    return encode_binop(arguments, RSB);
}

Instruction encode_and(StringArray arguments) {
    return encode_binop(arguments, AND);
}

Instruction encode_eor(StringArray arguments) {
    return encode_binop(arguments, EOR);
}

Instruction encode_orr(StringArray arguments) {
    return encode_binop(arguments, ORR);
}

/* DATA ALLOCATION */

Instruction encode_mov(StringArray arguments) {
    char *Rd       = arguments.elements[0];
    char *operand2 = arguments.elements[1];

    // S = 0; Rn = 0;
    return encode_data_processing(MOV, 0, Rd, "r0", operand2);
}

/* TESTS */

Instruction encode_tst(StringArray arguments) {
    return encode_test(arguments, TST);
}

Instruction encode_teq(StringArray arguments) {
    return encode_test(arguments, TEQ);
}

Instruction encode_cmp(StringArray arguments) {
    return encode_test(arguments, CMP);
}

/* OPTIONAL */

Instruction encode_optional(char *Rd, char *Rn,
                            StringArray arguments, uint8_t opcode) {
    char *Rm           = arguments.elements[2];
    char *Rs           = arguments.elements[4];
    uint8_t shift_type = get_shift_type(arguments.elements[3]);

    Instruction instruction;
    instruction.cond = AL; // Always
    instruction.type = DATA_PROCESSING;

    instruction.data_processing.opcode   = opcode;
    instruction.data_processing.S        = 0;
    instruction.data_processing.Rd       = register_number_build(Rd);
    instruction.data_processing.Rn       = register_number_build(Rn);
    // I always zero because operand 2 is a register
    instruction.data_processing.I        = 0;

    uint16_t op2 = 0;

    if (Rs[0] == 'r') {
        // Shifted by a register
        op2 |= register_number_build(Rs) << 8;
        op2 |= 1 << 4;
    } else {
        // shifted by a constant amount
        op2 |= number_build(Rs) << 7;
    }
    op2 |= register_number_build(Rm);
    op2 |= shift_type << 5;

    instruction.data_processing.operand2 = op2;

    return instruction;
}

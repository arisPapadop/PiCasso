// Extern from assemble
uint32_t current_address = 0;

Instruction encode_branch(StringArray arguments, uint32_t cond) {
    Instruction instruction;
    instruction.cond = cond;
    instruction.type = BRANCH;

    char *label = arguments.elements[0];

    // Gets the address from the labels table
    uint32_t instr_address = get_address(label)->address;
    instruction.branch.offset =
           ((instr_address - current_address - 2 * WORD_SIZE) / 4) & 0x00FFFFFF;
    return instruction;
}

/* BRANCH INSTRUCTIONS */

Instruction encode_beq(StringArray arguments) {
    return encode_branch(arguments, EQ);
}

Instruction encode_bne(StringArray arguments) {
    return encode_branch(arguments, NE);
}

Instruction encode_bge(StringArray arguments) {
    return encode_branch(arguments, GE);
}

Instruction encode_blt(StringArray arguments) {
    return encode_branch(arguments, LT);
}

Instruction encode_bgt(StringArray arguments) {
    return encode_branch(arguments, GT);
}

Instruction encode_ble(StringArray arguments) {
    return encode_branch(arguments, LE);
}

Instruction encode_b(StringArray arguments) {
    return encode_branch(arguments, AL);
}
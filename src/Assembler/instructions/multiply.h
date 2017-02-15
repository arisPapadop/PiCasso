Instruction encode_multiply(StringArray arguments, uint8_t A) {

    Instruction instruction;
    instruction.cond = AL;
    instruction.type = MULTIPLY;

    char *Rd = arguments.elements[0];
    char *Rm = arguments.elements[1];
    char *Rs = arguments.elements[2];

    instruction.multiply.A  = A;
    instruction.multiply.S  = 0;
    instruction.multiply.Rd = register_number_build(Rd);
    instruction.multiply.Rm = register_number_build(Rm);
    instruction.multiply.Rs = register_number_build(Rs);
    instruction.multiply.Rn = 0;

    if (A) {
        char *Rn = arguments.elements[3];
        instruction.multiply.Rn = register_number_build(Rn);
    }

    return instruction;
}


/* MULTIPLY INSTRUCTIONS */

Instruction encode_mul(StringArray arguments) {
    // A = 0;
    return encode_multiply(arguments, 0);
}

Instruction encode_mla(StringArray arguments) {
    // A = 1;
    return encode_multiply(arguments, 1);
}

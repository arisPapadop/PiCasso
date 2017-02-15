Instruction encode_halt(StringArray arguments) {
    Instruction instruction;
    instruction.type = HALT;
    return instruction;
}

Instruction encode_left_shift(StringArray arguments) {

    char *Rn         = arguments.elements[0];
    char *expression = arguments.elements[1];

    Instruction instruction;

    instruction.cond = AL; // Always
    instruction.type = LEFT_SHIFT;

    instruction.left_shift.opcode   = MOV;
    instruction.left_shift.I        = 0;
    instruction.left_shift.Rd       = register_number_build(Rn);
    instruction.left_shift.Rn       = 0;
    instruction.left_shift.S        = 0;
    instruction.left_shift.shift    = (uint8_t)number_build(expression);
    instruction.left_shift.Rm       = register_number_build(Rn);

    return instruction;
}

// Externs used
uint32_t end_of_file;
uint32_t current_address;
Memory_Data *memory;
Memory_Data *memory_ptr;

Instruction encode_single_data_transfer(uint8_t L, StringArray arguments) {
    Instruction instruction;
    instruction.cond = AL;
    instruction.type = SINGLE_DATA_TRANSFER;

    // Default value for U is 1
    instruction.single_data_transfer.U = 1;

    char *Rd = arguments.elements[0];
    instruction.single_data_transfer.Rd = register_number_build(Rd);

    instruction.single_data_transfer.L = L;

    StringArray expression;
    expression.elements = &(arguments.elements[1]);
    expression.size     = arguments.size - 2;

    if (expression.elements[0][0] == '=') {

        // Immediate offset - will always be pre indexed
        instruction.single_data_transfer.P = 1;

        // Need to set I to zero for immediate
        uint32_t offset = number_build(expression.elements[0]);
        instruction.single_data_transfer.I = 0;

        if (offset < 255) {
            // Treat as mov if offset less than 255
            instruction = encode_mov(arguments);
            instruction.data_processing.I = 1;
        } else {
            // Must put offset in 4 bytes at end of file
            instruction.single_data_transfer.Rn     =  PC_REGISTER_NUMBER;

            Memory_Data memory_data;
            memory_data.address = end_of_file;
            memory_data.value   = offset;
            *memory = memory_data;
            memory++;

            offset = end_of_file - current_address - (2 * WORD_SIZE);
            end_of_file += WORD_SIZE;
            instruction.single_data_transfer.offset = offset;
        }

    } else {
        instruction.single_data_transfer.I  = 0;
        instruction.single_data_transfer.Rd = register_number_build(Rd);

        // Rn is always the first thing in the list of expressions
        char *Rn = expression.elements[0];
        instruction.single_data_transfer.Rn = register_number_build(Rn);

        if (expression.size == 1) {
            // Using base register Rn with an offset of zero
            // Pre indexing
            instruction.single_data_transfer.P = 1;
            instruction.single_data_transfer.offset = 0;

        } else if (expression.size == 2) {
            if (Rn[get_length(Rn) - 1] != ']') {
                // [Rn, #expression]
                // Pre indexing
                instruction.single_data_transfer.P = 1;
            } else {
                // [Rn], #expression
                // Post indexing
                instruction.single_data_transfer.P = 0;
            }

            // Check if the offset is a register or a direct number
            if (is_register(expression.elements[1])) {
                instruction.single_data_transfer.I = 1;
            } else {
                instruction.single_data_transfer.I = 0;
            }

            int offset = number_build(expression.elements[1]);
            if (offset < 0) {
                // Change the U bit and set the offset to positive.
                instruction.single_data_transfer.U = 0;
                offset *= -1;
            }
            check_rotations(offset, &(instruction.single_data_transfer.offset));

        } else {
            // More than 2 registers
            instruction.single_data_transfer.P = 1;
            instruction.single_data_transfer.U = 1;

            if (expression.elements[3][0] == 'r') {
                // Shifted by a register
                instruction.single_data_transfer.I = 0;
            } else {
                // Shifted by a number
                instruction.single_data_transfer.I = 1;
                char *Rm    = expression.elements[1];
                char *shift = expression.elements[3];
                uint8_t shift_type = get_shift_type(expression.elements[2]);

                uint16_t offset = 0;
                offset |= register_number_build(Rm);
                offset |= number_build(shift) << 7;
                offset |= shift_type << 5;
                instruction.single_data_transfer.offset = offset;
            }
        }
    }

    return instruction;
}

Instruction encode_ldr(StringArray arguments) {
    // L = 1; Rn = 0;
    return encode_single_data_transfer(1, arguments);
}

Instruction encode_str(StringArray arguments) {
    // S = 0
    return encode_single_data_transfer(0, arguments);
}

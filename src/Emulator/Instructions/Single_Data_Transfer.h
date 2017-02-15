void execute_single_data_transfer(State *state, Single_Data_Transfer *instruction) {
    Shift_Struct shift_struct;

    uint16_t offset = (*instruction).offset;

    uint32_t shift_by;

    // Where we are storing or loading to
    uint32_t mem_address = (*state).registers.general_purpose[(*instruction).Rn];

    uint32_t shifted_mem;
    uint32_t final_address;

    // What we are storing or loading
    uint32_t data;

    if ((*instruction).I) {
        // Offset interpreted as a shifted register
        // Gives us back a structure - need shift
        shift_struct = shift_op2(state, offset);
    } else {
        // Unsigned 12 bit immediate offset
        shift_struct = shift_immed(offset);
    }

    shift_by = shift_struct.shift;

    if ((*instruction).U) {
        // Add what we shift by
        shifted_mem = mem_address + shift_by;
    } else {
        // Subtract by what we shift by
        shifted_mem = mem_address - shift_by;
    }

    if ((*instruction).L) {

        if ((*instruction).P) {
            final_address = shifted_mem;
        } else {
            final_address = mem_address;
        }

        // If it is out of bounds and not a gpio access do nothing
        if (!memory_in_bounds(final_address)
            && !(gpio_access(final_address))) {
            return;
        }

        // If it is a gpio access we directly move the address
        if (gpio_access(final_address)) {
            data = final_address;
        } else {
            data = word_build(&((*state).memory[final_address]));
        }

        // Store the data in the correct register
        (*state).registers.general_purpose[((*instruction).Rd)] = data;

    } else {
        data = (*state).registers.general_purpose[((*instruction).Rd)];

        if ((*instruction).P) {
          if (!memory_in_bounds(shifted_mem)) {
            return;
          }
            store_word_in_memory(data, &((*state).memory[shifted_mem]));
        } else {
          if (!memory_in_bounds(mem_address)) {
            return;
          }
            store_word_in_memory(data, &((*state).memory[mem_address]));
        }
    }

    if ((*instruction).P) {
        // Offset is added/subtracted to base register before transferring
        // Spec says pre indexing is not going to change the value Rn
        // Nothing happens
    } else {
        // Offset is added/subtracted to base register after transferring
        // Change contents of base register by offset
        // Apparently this happens after we have moved the data?

        // Replace the value in the base register with the new address
        (*state).registers.general_purpose[(*instruction).Rn] = shifted_mem;
    }
}

void execute_multiply(State *state, Multiply *instruction) {
  uint32_t *GP_registers = (*state).registers.general_purpose;

  uint32_t Rm = (*instruction).Rm;
  uint32_t Rs = (*instruction).Rs;
  uint32_t Rn = (*instruction).Rn;
  uint32_t Rd = (*instruction).Rd;

  switch ((*instruction).A) {
    // Accumulate is set
    case 1: GP_registers[(*instruction).Rd] 
	    = GP_registers[Rm] * GP_registers[Rs] + GP_registers[Rn];
	    break;
    // Accumulate not set
    case 0: GP_registers[Rd] = GP_registers[Rm] * GP_registers[Rs];
  }
  
  if ((*instruction).S) {
    // If S bit was set
    (*state).registers.CPSR.N = get_bit(GP_registers[Rd], 31);
    (*state).registers.CPSR.Z = GP_registers[Rd] == 0;
  }
}

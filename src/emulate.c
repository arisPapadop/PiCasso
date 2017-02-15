#include "emulate.h"
/*---------------------------*//* EMULATOR *//*-------------------------------*/
int main(int argc, char **argv) {
  // If the user did not specify the correct number of arguments exit the
  // function.
  if (argc != 2) {
    printf("Error: Invalid number of arguments.");
    return 1;
  } else {
    // Read words from the binary file.
    FILE *file = fopen(argv[1], "rb");
    // If the file did not open correctly then exit the function.
    if (!file) {
      printf("Error: Unable to open the file.");
      return 1;
    }
    // Initialize the state and the pipeline to their respective static empty
    // constants, whose value is 0, by definition.
    state    = Empty_State;
    pipeline = Empty_Pipe;
    // Read all the data from the binary file and store them in the words of the
    // state struct, which represents the main memory and the CPU registers of
    // the ARM11.
    fread(state.memory, 1, bytes_count(file), file);
    fclose(file);

    // Fill the pipeline, to start the normal processing cycle.
    fill_pipeline();

    // The CPU Fetch-Decode-Execute loop, which runs until the decoded instruction
    // is HALT.
    do {
      execute();
      decode();
      fetch();
    } while (pipeline.decoded.type != HALT);
  }
  // Print the current state of the CPU and the Main Memory.
  print_registers(&state);
  return 0;
}

/*-----------------------*//* PIPELINE OPERATIONS *//*------------------------*/
// Fill the pipeline by adding and moving the first instruction to decode and
// the next one to fetch.
void fill_pipeline() {
  fetch();
  decode();
  fetch();
}

// Empty the pipeline, after a branch instruction is executed.
void empty_pipeline() {
  pipeline = Empty_Pipe;
}

/*------------------------------*//* FETCH *//*-------------------------------*/
// Add the next instruction to the pipeline, using word_build, from util.h
void fetch() {
  pipeline.fetched = word_build(&(state.memory[state.registers.PC]));

  // Increment program counter, at the beginning of each cycle.
  state.registers.PC = state.registers.PC + WORD_SIZE;
}

/*-----------------------------*//* DECODE *//*-------------------------------*/
// The blueprint of the functions for each instruction is the same. First set
// the enum type of that instruction and then all bits to their corresponding
// bits from the encoded instruction.
void decode_branch(Instruction *i, uint32_t encoded) {
  i -> type = BRANCH;

  i -> branch.offset = get_bits(encoded, 0, 23);
}

void decode_single_data_transfer(Instruction *i, uint32_t encoded) {
  i -> type = SINGLE_DATA_TRANSFER;

  i -> single_data_transfer.I      =  (uint8_t)get_bits(encoded, 25, 25);
  i -> single_data_transfer.P      =  (uint8_t)get_bits(encoded, 24, 24);
  i -> single_data_transfer.U      =  (uint8_t)get_bits(encoded, 23, 23);
  i -> single_data_transfer.L      =  (uint8_t)get_bits(encoded, 20, 20);
  i -> single_data_transfer.Rn     =  (uint8_t)get_bits(encoded, 16, 19);
  i -> single_data_transfer.Rd     =  (uint8_t)get_bits(encoded, 12, 15);
  i -> single_data_transfer.offset = (uint16_t)get_bits(encoded,  0, 11);
}

void decode_data_processing(Instruction *i, uint32_t encoded) {
  i -> type = DATA_PROCESSING;

  i -> data_processing.I        =  (uint8_t)get_bits(encoded, 25, 25);
  i -> data_processing.opcode   =  (uint8_t)get_bits(encoded, 21, 24);
  i -> data_processing.S        =  (uint8_t)get_bits(encoded, 20, 20);
  i -> data_processing.Rn       =  (uint8_t)get_bits(encoded, 16, 19);
  i -> data_processing.Rd       =  (uint8_t)get_bits(encoded, 12, 15);
  i -> data_processing.operand2 = (uint16_t)get_bits(encoded,  0, 11);
}

void decode_multiply(Instruction *i, uint32_t encoded) {
  i -> type = MULTIPLY;

  i -> multiply.A  = (uint8_t)get_bits(encoded, 21, 21);
  i -> multiply.S  = (uint8_t)get_bits(encoded, 20, 20);
  i -> multiply.Rd = (uint8_t)get_bits(encoded, 16, 19);
  i -> multiply.Rn = (uint8_t)get_bits(encoded, 12, 15);
  i -> multiply.Rs = (uint8_t)get_bits(encoded,  8, 11);
  i -> multiply.Rm = (uint8_t)get_bits(encoded,  0, 3);
}

// Distinguish between the different instruction types and then build the
// instruction using the designated function.
void decode() {
  uint32_t    encoded = (uint32_t)pipeline.fetched;
  Instruction decoded;

  if (encoded == 0) {
    decoded.type = HALT;
  } else {
    decoded.cond = (uint8_t)get_bits(encoded, COND_START, COND_END);

    int significant = get_bits(encoded, 26, 27);

    switch (significant) {
      case 2:
        decode_branch(&decoded, encoded);
            break;
      case 1:
        decode_single_data_transfer(&decoded, encoded);
            break;
      case 0:
        if (get_bit(encoded, 25)) {
          decode_data_processing(&decoded, encoded);
        } else {
          if (get_bits(encoded, 4, 7) == 9) {
            decode_multiply(&decoded, encoded);
          } else {
            decode_data_processing(&decoded, encoded);
          }

        }
            break;
      default:
            break;
    }
  }
  // Add the decoded instruction to the pipeline.
  pipeline.decoded = decoded;
}

/*-----------------------------*//* EXECUTE *//*------------------------------*/
// Check the condition of the instruction by inspecting the CPSR bits.
bool Z_set()      { return state.registers.CPSR.Z; }
bool N_equals_V() { return state.registers.CPSR.N == state.registers.CPSR.V; }

bool is_valid(int condition) {
  switch (condition) {
    case EQ:
      return Z_set();
    case NE:
      return !Z_set();
    case GE:
      return N_equals_V();
    case LT:
      return !N_equals_V();
    case GT:
      return !Z_set() && N_equals_V();
    case LE:
      return Z_set() || !N_equals_V();
    default:
      return true;
  }
}

// Take the instruction from the pipeline and if the condition is satisfied
// execute it using the specific execute function.
void execute() {
  Instruction to_execute = pipeline.decoded;

  if (is_valid(to_execute.cond)) {
    // Execute the instruction
    switch (to_execute.type) {
      case BRANCH:
        // If the instruction is a branch, empty the pipeline and then fetch
        // once. Then the normal processing loop takes care of filling the
        // pipeline, as it skips the execution of an empty instruction.
        execute_branch(&state, &(to_execute.branch));
        empty_pipeline();
        fetch();
        break;
      case DATA_PROCESSING:
        execute_data_processing(&state, &(to_execute.data_processing));
        break;
      case MULTIPLY:
        execute_multiply(&state, &(to_execute.multiply));
        break;
      case SINGLE_DATA_TRANSFER:
        execute_single_data_transfer(&state, &(to_execute.single_data_transfer));
        break;
      case HALT:
        // The program ends when halt is decoded so this is essentially a
        // "Should not get here" error.
        printf("Error: Tried to execute the 'halt' instruction.");
        break;
      default: break;
    }
  }
}

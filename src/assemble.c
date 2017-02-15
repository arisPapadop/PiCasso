#include "assemble.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Error: Invalid number of arguments");
  } else {
    // Constructs 'mnemonic to function' hash table
    initialise();
    //free_tables();
    //initialise();
    char const *const input_file_name  = argv[1];
    char const *const output_file_name = argv[2];
    FILE *input = fopen(input_file_name, "r");

/*-----------------------------*//* FIRST PASS *//*---------------------------*/
    // Holds each line as it is read from file
    char line[MAX_LINE_SIZE];

    // The address of the current line being read
    uint32_t line_address = 0;

    // Reads from file line by line
    while (fgets(line, MAX_LINE_SIZE, input)) {
      if (is_label(line)) {
        // Line represents a label:
        // Remove ':' from label to leave label name
        line[strlen(line) - 2] = '\0';

        put_address(line, line_address);
      } else if (strcmp(line, "\n") != 0) {
        // Line represents instruction:
        line_address += WORD_SIZE;
      }
    }

    end_of_file = line_address;

    Memory_Data *values = (Memory_Data *) malloc(sizeof(Memory_Data) * end_of_file);
    memory = values;

/*----------------------------*//* SECOND PASS *//*---------------------------*/
    // Reset for next pass
    rewind(input);

    printf("The end of the file is %d\n", end_of_file);

    // Open destination file
    FILE *output = fopen(output_file_name, "wb");

    while (fgets(line, sizeof(line), input)) {
      if (!is_label(line) && strcmp(line, "\n") != 0) {
        //String handling on line (split into mnemonic and arguments)
        Token instr = tokenize(line);

        // Uses 'mnemonic to function' hash table to assemble the line
        Instruction encoded = get(instr.mnemonic)->function(instr.arguments);

        // Uses the instruction structure to assemble the instruciton
        uint32_t assembled = assemble(encoded);

        // Write the assembled line to output file
        fwrite(&assembled, WORD_SIZE, 1, output);

        // Change endianness
        uint32_t to_print = 0;
        for (int i = WORD_SIZE - 1; i >= 0; --i) {
          uint32_t byte = byte_from_word(assembled, i);
          to_print |= byte << (BYTE_SIZE * i);
        }
        print_binary(to_print);
        free(instr.arguments.elements);

        current_address += WORD_SIZE;
      }
    }

    // Display memory array
    Memory_Data *to_free = values;
    while (values != memory) {
      printf("%d: %d\n", values->address, values->value);
      fwrite(&(values->value), WORD_SIZE, 1, output);
      values++;
    }

    free(to_free);

    printf("Reached here\n");
    free_tables();
    printf("Tables are freed\n");
    // Close files after use
    fclose(output);
    fclose(input);
  }
}

/*---------------------*//* MNEMONIC TO FUNCTION TABLE *//*-------------------*/
// The hardcoded lookup table which associaters the function mnemonics with the
// correspinding ecoding functions is initialized here by adding all mnemonics
// and function pointers to an empty hash-table.
void initialise() {
  put_function("andeq", &encode_halt);
  put_function("add", &encode_add);
  put_function("sub", &encode_sub);
  put_function("rsb", &encode_rsb);
  put_function("and", &encode_and);
  put_function("eor", &encode_eor);
  put_function("orr", &encode_orr);
  put_function("mov", &encode_mov);
  put_function("tst", &encode_tst);
  put_function("teq", &encode_teq);
  put_function("cmp", &encode_cmp);
  put_function("mul", &encode_mul);
  put_function("mla", &encode_mla);
  put_function("ldr", &encode_ldr);
  put_function("str", &encode_str);
  put_function("beq", &encode_beq);
  put_function("bne", &encode_bne);
  put_function("bge", &encode_bge);
  put_function("blt", &encode_blt);
  put_function("bgt", &encode_bgt);
  put_function("ble", &encode_ble);
  put_function("b"  , &encode_b);
  put_function("lsl", &encode_left_shift);
}

/*------------------------------*//* ASSEMBLE *//*----------------------------*/

uint32_t assemble_branch(Branch branch) {
  uint32_t result = 0;

  result |= branch.offset;
  result |= 10 << 24;

  return result;
}

uint32_t assemble_data_processing(Data_Processing data_processing) {
  uint32_t result = 0;

  result |= data_processing.operand2;
  result |= data_processing.Rd     << 12;
  result |= data_processing.Rn     << 16;
  result |= data_processing.S      << 20;
  result |= data_processing.opcode << 21;
  result |= data_processing.I      << 25;

  return result;
}

uint32_t assemble_multiply(Multiply multiply) {
  uint32_t result = 0;

  result |= multiply.Rm;
  result |= multiply.Rs << 8;
  result |= multiply.Rn << 12;
  result |= multiply.Rd << 16;
  result |= multiply.S  << 20;
  result |= multiply.A  << 21;

  // To distinguish Multiply from Data Processing
  result |= 9 << 4;

  return result;
}

uint32_t assemble_single_data_transfer(Single_Data_Transfer single_data_transfer) {
  uint32_t result = 0;

  result |= single_data_transfer.offset;
  result |= single_data_transfer.Rd << 12;
  result |= single_data_transfer.Rn << 16;
  result |= single_data_transfer.L  << 20;
  result |= single_data_transfer.U  << 23;
  result |= single_data_transfer.P  << 24;
  result |= single_data_transfer.I  << 25;

  result |= 1 << 26;

  return result;
}

uint32_t assemble_lsl(Left_Shift lsl) {
  uint32_t result = 0;

  result |= lsl.Rd     << 12;
  result |= lsl.Rn     << 16;
  result |= lsl.S      << 20;
  result |= lsl.opcode << 21;
  result |= lsl.I      << 25;
  result |= lsl.shift  << 7;
  result |= lsl.Rm;

  return result;
}

uint32_t assemble(Instruction instruction) {
  uint32_t result = 0;

  // Initialise result with the condition.
  result = instruction.cond << COND_START;

  switch (instruction.type) {
    case BRANCH:
      result |= assemble_branch(instruction.branch);
          break;
    case DATA_PROCESSING:
      result |= assemble_data_processing(instruction.data_processing);
          break;
    case MULTIPLY:
      result |= assemble_multiply(instruction.multiply);
          break;
    case SINGLE_DATA_TRANSFER:
      result |= assemble_single_data_transfer(instruction.single_data_transfer);
          break;
    case LEFT_SHIFT:
      result |= assemble_lsl(instruction.left_shift);
          break;
    case HALT:
      result = 0;
          break;
  }

  return result;
}

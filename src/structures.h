// Stores the state of the registers and the emulated memory.
typedef struct Cpsr {
  bool N, Z, C, V;
} Cpsr;

typedef struct Registers {
  uint32_t general_purpose[13];
  uint32_t SP, LR, PC;
  Cpsr CPSR;
} Registers;

typedef struct State {
  uint8_t memory[CAPACITY];
  Registers registers;
  Registers *r;
} State;

// Data Processing
typedef struct Data_Processing {
  uint8_t Rn;
  uint8_t Rd;
  uint16_t operand2;
  uint8_t I;
  uint8_t S;
  uint8_t opcode;
} Data_Processing;

// Multiply
typedef struct Multiply {
  uint8_t Rd;
  uint8_t Rn;
  uint8_t Rs;
  uint8_t Rm;
  uint8_t A;
  uint8_t S;
} Multiply;

// Single Data Transfer
typedef struct Single_Data_Transfer {
  uint8_t Rn;
  uint8_t Rd;
  uint16_t offset;
  uint8_t I;
  uint8_t P;
  uint8_t U;
  uint8_t L;
} Single_Data_Transfer;

// Branch
typedef struct Branch {
  int offset;
} Branch;

typedef struct Left_Shift {
    uint8_t Rn;
    uint8_t Rd;
    uint8_t shift;
    uint8_t I;
    uint8_t S;
    uint8_t opcode;
    uint8_t Rm;
} Left_Shift;


// Instruction Types
typedef enum Instruction_Type {
  DATA_PROCESSING,
  MULTIPLY,
  SINGLE_DATA_TRANSFER,
  BRANCH,
  LEFT_SHIFT,
  HALT
} Instruction_Type;

// Instruction Format
typedef struct Instruction {
  uint8_t          cond;
  Instruction_Type type;

  Data_Processing      data_processing;
  Multiply             multiply;
  Single_Data_Transfer single_data_transfer;
  Branch               branch;
  Left_Shift           left_shift;
} Instruction;

// Used to handle the fetch-execute cycle
typedef struct Pipeline {
  uint32_t    fetched;
  Instruction decoded;
} Pipeline;

// Used so we can return the shift amount and C flag
typedef struct Shift_Struct {
  uint32_t    shift;
  uint8_t     C;
} Shift_Struct;

// To check rotation regarding shifts
typedef struct Rotation {
    uint32_t result;
    uint8_t  rotation;
} Rotation;

// For analysing arguments given to assembler
typedef struct StringArray {
    uint32_t size;
    char**   elements;
} StringArray;

// Used in hashtable for assembler
typedef struct Token {
    char        *mnemonic;
    StringArray arguments;
} Token;

// Used when storing data at end of program
typedef struct Memory_Data {
  int address;
  uint32_t value;
} Memory_Data;
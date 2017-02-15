// General constants
#define CAPACITY            65536
#define WORD_SIZE           4
#define GP_REGISTERS        13
#define PC_REGISTER_NUMBER  15
#define BYTE_SIZE           8
#define END_OF_STRING       '\0'

// Conditions
#define COND_START 28
#define COND_END   31
#define EQ 0
#define NE 1
#define GE 10
#define LT 11
#define GT 12
#define LE 13
#define AL 14

// Data_Processing Opcodes
#define AND 0
#define EOR 1
#define SUB 2
#define RSB 3
#define ADD 4
#define TST 8
#define TEQ 9
#define CMP 10
#define ORR 12
#define MOV 13

// GPIO Pin Memories
#define GPIO_ZERO   538968064
#define GPIO_TEN    538968068
#define GPIO_TWENTY 538968072
#define GPIO_OFF    538968104
#define GPIO_ON     538968092

//------------------------------------------

// Shift Types
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

/*-------------------------*//* STANDARD LIBRARIES *//*-----------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

/*--------------------------*//* LOCAL LIBRARIES *//*-------------------------*/
#include "constants.h"
#include "structures.h"
#include "Util Libraries/common_util.c"
#include "Util Libraries/assembler_util.c"
#include "Assembler/hash_table.h"
#include "Assembler/instructions/branch.h"
#include "Assembler/instructions/data_processing.h"
#include "Assembler/instructions/multiply.h"
#include "Assembler/instructions/single_data_transfer.h"
#include "Assembler/instructions/special.h"

/*--------------------------*//* GLOBAL VARIABLES *//*------------------------*/

extern uint32_t end_of_file;
extern uint32_t current_address;
extern Memory_Data *memory;

/*-----------------------------*//* DEFINITIONS *//*--------------------------*/
#define MAX_LINE_SIZE     511

/*------------------------*//* FUNCTION PROTOTYPES *//*-----------------------*/

void initialise();
uint32_t assemble(Instruction instruction);
bool is_label(char *line);
void initialise();
uint32_t assemble_branch(Branch branch);
uint32_t assemble_data_processing(Data_Processing data_processing);
uint32_t assemble_single_data_transfer(Single_Data_Transfer single_data_transfer);
uint32_t assemble_lsl(Left_Shift lsl);
uint32_t assemble(Instruction instruction);

/*----------------------------------------------------------------------------*/
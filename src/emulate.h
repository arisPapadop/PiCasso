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
#include "Util Libraries/emulator_util.c"
#include "Emulator/Instructions/Branch.h"
#include "Emulator/Instructions/Data_Processing.h"
#include "Emulator/Instructions/Multiply.h"
#include "Emulator/Instructions/Single_Data_Transfer.h"

/*------------------------*//* FUNCTION PROTOTYPES *//*-----------------------*/
void fill_pipeline();
void empty_pipeline();

void fetch();
void decode();
void execute();

void decode_branch(Instruction *i, uint32_t encoded);
void decode_single_data_transfer(Instruction *i, uint32_t encoded);
void decode_data_processing(Instruction *i, uint32_t encoded);
void decode_multiply(Instruction *i, uint32_t encoded);

bool Z_set();
bool N_equals_V();
bool is_valid(int condition);

/*-----------------------*//* GLOBAL VARIABLES *//*---------------------------*/
static const Pipeline Empty_Pipe;
static const State Empty_State;
Pipeline pipeline;
State state;

/*----------------------------------------------------------------------------*/
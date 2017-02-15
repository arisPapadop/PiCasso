/*------------------------*//* STANDARD LIBRARIES *//*-----------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <wiringPi.h> // GPIO
#include <mcp23017.h> // Shift registers

/*--------------------------*//* LOCAL LIBRARIES *//*------------------------*/
#include "gpio.h"

#include "util.c"
#include "gallery_control.c"

#include "draw.c"
#include "move.c"
#include "gallery.c"

#include "letters.c"

/*------------------------------*//* MODES *//*------------------------------*/
typedef enum mode
{
	DRAW,
	MOVE,
	GALLERY,
	UNKNOWN
} mode_t;

/*-----------------------*//* FUNCTION PROTOTYPES *//*-----------------------*/
void blank_screen();
void initialise();
void detect();

/*---------------------------------------------------------------------------*/

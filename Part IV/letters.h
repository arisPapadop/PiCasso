/*---------------------------*//* DEFINITIONS *//*---------------------------*/
#define STARTING_ROW    5
#define STARTING_COLUMN 3
#define LETTER_HEIGHT   7

#define DISPLAY_CYCLES 2

/*---------------------------*//* STRUCTURES *//*---------------------------*/
typedef struct letter 
{
    uint32_t rows[LETTER_HEIGHT];
} letter_t;

/*-----------------------*//* FUNCTION PROTOTYPES *//*-----------------------*/
void display_letter(image_t *image, char c);
letter_t make_letter(char c);

/*----------------------------------------------------------------------------*/
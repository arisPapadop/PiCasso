#include "letters.h"

void display_letter(image_t *image, char c)
{
	image_t original_image = *image;

	letter_t letter = make_letter(c);
	for (int i = 0; i < LETTER_HEIGHT; i++)
	{
		// Modify the image
		uint32_t row = (letter.rows)[i] << (BITS_X_LED * STARTING_COLUMN);
		(image -> rows)[STARTING_ROW + i] = row;
	}

	// Set the rest of the rows in the image to zero
	for (int i = 0; i < STARTING_ROW; i++)
		(image -> rows)[i] = 0;
	for (int i = STARTING_ROW + LETTER_HEIGHT; i < SIZE; i++)
		(image -> rows)[i] = 0;

	for (int i = 0; i < DISPLAY_CYCLES; i++)
		display(image);

	*image = original_image;
}

letter_t make_letter(char c) 
{
    letter_t l;
    switch (c) 
    {
        case 'D': 
            l.rows[0] = 0x0FF0;
            l.rows[1] = 0x3030;
            l.rows[2] = 0x3030;
            l.rows[3] = 0x3030;
            l.rows[4] = 0x3030;
            l.rows[5] = 0x3030;
            l.rows[6] = 0x0FF0;
            return l;
        case 'G':
            l.rows[0] = 0x0FC0; 
            l.rows[1] = 0x3030; 
            l.rows[2] = 0x0030; 
            l.rows[3] = 0x3C30; 
            l.rows[4] = 0x3030; 
            l.rows[5] = 0x3030; 
            l.rows[6] = 0x0FC0;
            return l;
        case 'M': 
            l.rows[0] = 0xC0C0;
            l.rows[1] = 0xF3C0;
            l.rows[2] = 0xCCC0;
            l.rows[3] = 0xC0C0;
            l.rows[4] = 0xC0C0;
            l.rows[5] = 0xC0C0;
            l.rows[6] = 0xC0C0;
            return l;
        default:
            perror("Letter not implemented.");
            exit(1);
    }
}
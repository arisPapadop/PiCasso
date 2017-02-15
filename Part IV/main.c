#include "main.h"

image_t *image_displayed;

extern gallery_t *video_gallery;

extern bool *columns;
extern bool should_flash;
extern bool ready_to_play;
extern int led_count;
extern led_t *drawn_leds;

mode_t mode = UNKNOWN;

int main(void)
{
	initialise();
	for (;;)
	{
		detect();
		display(image_displayed);
	}
}

/*--------------------------*//* HARDWARE SETUP *//*-------------------------*/
void blank_screen()
{
	
	drawn_leds = malloc(sizeof(led_t) * MAX_LEDS);

	image_displayed = calloc(1, sizeof(image_t));
	image_displayed -> last_led  = calloc(1, sizeof(led_t));
	image_displayed -> led_count = 0;
	for (int i = 0; i < SIZE; i++)
	{
		columns[i] = HIGH;
		digitalWrite(X_BASE + i, HIGH);
		digitalWrite(Y_BASE + i, LOW);
	}
}

void initialise()
{
	video_gallery = make_gallery();
	
	// Set up wiringPi
	if (wiringPiSetup() == -1)
	{
		printf("Error: Couldn't load wiringPi.\n");
		exit(1);
	}

	// Set up the expanders
	mcp23017Setup(X_BASE, X_ADDRESS);
	mcp23017Setup(Y_BASE, Y_ADDRESS);

	// Set pins as outputs
	for (int i = 0; i < SIZE; i++)
	{
		pinMode(X_BASE + i, OUTPUT);
		pinMode(Y_BASE + i, OUTPUT);
	}

	// Switch off all leds
	columns = malloc(sizeof(bool) * SIZE);
	blank_screen();

	// Set up the buttons + pull up
	pinMode(DOWN_BTN,  INPUT); 
	pinMode(UP_BTN,    INPUT); 
	pinMode(RIGHT_BTN, INPUT); 
	pinMode(LEFT_BTN,  INPUT); 
	pullUpDnControl(DOWN_BTN,  PUD_DOWN);
	pullUpDnControl(UP_BTN,    PUD_DOWN);
	pullUpDnControl(RIGHT_BTN, PUD_DOWN);
	pullUpDnControl(LEFT_BTN,  PUD_DOWN);

	// Control buttons
	pinMode(PEN_DOWN, INPUT);
	pinMode(MODE,     INPUT);
	pinMode(SAVE,     INPUT);
	pinMode(UNDO,     INPUT);
	pullUpDnControl(PEN_DOWN, PUD_DOWN);
	pullUpDnControl(MODE,     PUD_DOWN);
	pullUpDnControl(SAVE,     PUD_DOWN);
	pullUpDnControl(UNDO,     PUD_DOWN);
}

/*-----------------------*//* DELEGATION TO MODES *//*-----------------------*/
void detect()
{
	if (!digitalRead(MODE))
	{
		// Gallery mode
		if (mode == DRAW)
			move(image_displayed);

		if (mode != GALLERY)
		{	
			// First time in gallery
			display_letter(image_displayed, 'G');
			store();
		}

		mode = GALLERY;
		should_flash = false;
	}
	else
	{
		ready_to_play = false;

		// Only go to move mode if it is not an empty image
		if (digitalRead(SAVE) && image_displayed -> led_count > 0)
		{	
			// Move mode
			if (mode != MOVE)
			{
				// First time in move
				display_letter(image_displayed, 'M');
				free(drawn_leds);
			}

			mode = MOVE;
			should_flash = false;
		}
		else if (mode != MOVE)
		{
			// Draw mode
			if (mode != DRAW)
			{
				// First time in draw
				display_letter(image_displayed, 'D');

				free(image_displayed -> last_led);
				free(image_displayed);
				blank_screen();
			}

			// Undo
			if (digitalRead(UNDO) && led_count > 0)
			{
				*(image_displayed -> last_led) = drawn_leds[led_count - 1];
				set_led(image_displayed, drawn_leds[led_count - 1], false);
			}

			mode = DRAW;
			should_flash = true;
		}
	}

	// Detect logic
	switch (mode)
	{
		case DRAW: 
			draw(image_displayed);   
			break;
		case MOVE: 
			move(image_displayed);
			break;
		case GALLERY: 
			gallery(image_displayed); 
			break;
	}
}

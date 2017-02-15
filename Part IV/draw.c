#include "draw.h"


void draw(image_t *image)
{
	led_t *last = image -> last_led;

	// Detect direction
	int dy = digitalRead(DOWN_BTN)  - digitalRead(UP_BTN);
	int dx = digitalRead(RIGHT_BTN) - digitalRead(LEFT_BTN);

	if (dx != 0 || dy != 0)
	{
		// Change the last LED
		if (digitalRead(PEN_DOWN))
			set_led(image, *last, true);

		// Only move if in bounds
		if (in_bounds(last -> x, dx)) 
			last -> x += dx;
		if (in_bounds(last -> y, dy)){
			last -> y += dy;
		}
	}
}

int in_bounds(int coordinate, int displacement)
{
	if (displacement < 0)
		return (coordinate > 0);
	if (displacement > 0)
		return (coordinate < SIZE - 1);

	return 1;
}

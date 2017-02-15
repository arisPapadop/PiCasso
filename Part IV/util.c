#include "util.h"

bool *columns;
bool should_flash;

/*-------------------------------*//* GRADIENT *//*--------------------------*/
gradient_t next_gradient(gradient_t grad)
{
	switch (grad)
	{
		case OFF   : return GRAD_1;
		case GRAD_1: return GRAD_2;
		case GRAD_2: return GRAD_3;
		case GRAD_3: return GRAD_3;
	}
}

gradient_t opposite_gradient(gradient_t grad)
{
	switch (grad)
	{
		case OFF   : return GRAD_3;
		case GRAD_1: return GRAD_2;
		case GRAD_2: return GRAD_1;
		case GRAD_3: return OFF;
	}
}

gradient_t led_gradient(uint32_t leds_row, int pos)
{
	// Returns the 2 bits that represent the gradient of the specific led.
	int real_pos = BITS_X_LED * (SIZE - 1 - pos);
	return (gradient_t)(GET_BITS(leds_row, real_pos, real_pos + 1));
}

/*----------------------------*//* DISPLAY *//*------------------------------*/
void switch_on_row(int row, uint32_t leds_row, gradient_t current_grad)
{
	row = SIZE - 1 - row;

	digitalWrite(Y_BASE + row, HIGH);

	for (int column = 0; column < SIZE; column++)
	{
		// Only switch if led gradient is less than or equal then the current
		// gradient. This will make the intervals of the "LOW" be switched on
		// for smaller intervals than the "HIGH" which will make them look
		// dimmer.
		gradient_t grad = led_gradient(leds_row, column);

		bool value = grad < current_grad;

		if (value != columns[column])
		{
			digitalWrite(X_BASE + column, value);
			columns[column] = value;
		}

	}
}

void switch_off_row(int row)
{
	row = SIZE - 1 - row;
	for (int column = 0; column < SIZE; column++)
	{
		if (!columns[column])
		{
			columns[column] = HIGH;
			digitalWrite(X_BASE + column, HIGH);
		}
	}
	
	digitalWrite(Y_BASE + row, LOW);
}

void display(image_t *image_to_display)
{
	for (int i = 0; i < DETECTION_RATE + 
                            RATE_ADJUSTMENT(image_to_diplay -> led_count); i++)
	{
		// Switch on and off every row
		for (int row = 0; row < SIZE; row++)
		{
			uint32_t leds_row = (image_to_display -> rows)[row];
			
			led_t *last = image_to_display -> last_led;

			// Flashing led
			if (should_flash)
			{
				if (last -> y == row) 
				{
					(last -> flash) += 4;
					uint32_t mask = GRAD_3 << (BITS_X_LED * (last -> x));

					if (last -> flash > FLASHING_RATE)
					{
						leds_row |= mask;
					}
					else
					{
						leds_row &= ~mask;
					}
				}
			}

			if (leds_row != 0 || last -> y == row)
			{
				// Gradients logic
				for (gradient_t grad = GRAD_3; grad > OFF; grad--)
				{
					switch_on_row(row, leds_row, grad);
					delay(DELAY);
				}
				switch_off_row(row);
			}
			else
			{
				delay(GRAD_3 * DELAY);
			}
		}
	
	}
}

/*-------------------------*//* CHANGE IMAGE *//*----------------------------*/
void set_led(image_t *image_to_change, led_t led_to_set, bool set)
{
	int row    = led_to_set.y;
	int column = led_to_set.x;

	uint32_t *row_to_change = (image_to_change -> rows) + row;

	if (set)
	{
		if (image_to_change -> led_count < MAX_LEDS)
		{
			gradient_t current_grad = led_gradient(*row_to_change, column);
			gradient_t new_grad     = next_gradient(current_grad);

			uint32_t mask = ~(opposite_gradient(new_grad) << (BITS_X_LED * column));

			*row_to_change |= GRAD_3 << (BITS_X_LED * column);
			*row_to_change &= mask;

			drawn_leds[image_to_change -> led_count] = led_to_set;
			(image_to_change -> led_count)++;
		}
	}
	else
	{
		*row_to_change &= ~(GRAD_3 << (BITS_X_LED * column));

		(image_to_change -> led_count)--;
	}
}







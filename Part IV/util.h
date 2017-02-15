/*------------------------------*//* CONSTANTS *//*--------------------------*/
#define SIZE       16 // We assume that the LED matrix is a square
#define MAX_LEDS  100
#define BITS_X_LED  2
#define DELAY    0.07

#define GET_BITS(x, from, to) ((x >> from) & ((1 << (to - from + 1)) - 1))
#define RATE_ADJUSTMENT(x) (MAX_LEDS - x)

typedef enum 
{ 
	DETECTION_RATE = 5,
	FLASHING_RATE  = 128
} rates;

/*------------------------------*//* STRUCTURES *//*-------------------------*/
typedef struct led
{
	uint8_t x, y;
	uint8_t flash;
} led_t;

typedef struct image
{
	uint32_t rows[SIZE];
	led_t    *last_led;
	int      led_count;
} image_t;

typedef enum move
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    HALT
} move_t;

typedef enum gradient
{
	OFF,
	GRAD_1,
	GRAD_2,
	GRAD_3
} gradient_t;

/*-----------------------*//* FUNCTION PROTOTYPES *//*-----------------------*/
gradient_t next_gradient(gradient_t grad);
gradient_t next_gradient_cyclic(gradient_t grad);
gradient_t led_gradient(uint32_t leds_row, int pos);

void switch_on_row(int row, uint32_t leds_row, gradient_t current_grad);
void switch_off_row(int row);
void flash(int row, uint32_t *leds_row, image_t *image_to_display);
void display(image_t *image_to_display);

void set_led(image_t *image_to_change, led_t led_to_set, bool set);

/*---------------------------------------------------------------------------*/

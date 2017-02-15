/*-----------------------*//* FUNCTION PROTOTYPES *//*-----------------------*/
void move(image_t*);

static void move_internal(image_t *image, move_t move);
static void move_up(image_t *image);
static void move_down(image_t *image);
static void move_right(image_t *image);
static void move_left(image_t *image);

/*---------------------------------------------------------------------------*/
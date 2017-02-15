#include "move.h"

// Global variables
gallery_t *video_gallery;
frame_node_t *head_frame;
bool initialised = false;

image_t *original_image;

/*----------------------------*//* ANIMATIONS *//*---------------------------*/
void move(image_t *image)
{
    if (!initialised)
    {
        initialised = true;
        head_frame  = make_frame();

        original_image  = malloc(sizeof(image_t));
        *original_image = *image;
    }

    // Button detection.
    // Every time the user moves we keep track of the frames to then display
    // the video in the gallery.

    if (digitalRead(DOWN_BTN))
    {
        head_frame = add_frame(head_frame, DOWN);
        move_internal(image, DOWN);
    }
    else if (digitalRead(UP_BTN))
    {
        head_frame = add_frame(head_frame, UP);
        move_internal(image, UP);
    }
    else if (digitalRead(RIGHT_BTN))
    {
        head_frame = add_frame(head_frame, RIGHT);
        move_internal(image, RIGHT);
    }
    else if (digitalRead(LEFT_BTN))
    {
        head_frame = add_frame(head_frame, LEFT);
        move_internal(image, LEFT);
    } 
}

// Delegates to the different move functions
static void move_internal(image_t *image, move_t move)
{	
    switch(move){
        case UP:
            move_up(image);
            break;
        case DOWN:  
            move_down(image);
            break;
        case LEFT:  
            move_left(image);
            break;
        case RIGHT: 
            move_right(image);
            break;
        default: 
            perror("should not get here");
            break;
    }
}

// Rotates rows up
static void move_up(image_t *image)
{       
    int i = 0;

    uint32_t *image_rows = image -> rows;
    uint32_t temp = image_rows[i];

    while (i < SIZE - 1)
    {
        image_rows[i] = image_rows[i + 1];
        ++i;
    }

    image_rows[i] = temp;
}

// Rotates rows down
static void move_down(image_t *image)
{
    int i = SIZE - 1;
  
    uint32_t *image_rows = image -> rows;
    uint32_t temp =  image_rows[i];

    while (i > 0)
    {
        image_rows[i] = image_rows[i - 1];
        --i;
    }

    image_rows[i] = temp;
}

// Rotates columns right
static void move_right(image_t *image)
{
    uint32_t *image_rows = image -> rows;

    for (int i = 0; i < SIZE; i++)
    {
        // Execute left rotation
        image_rows[i] = (image_rows[i] << BITS_X_LED) | 
                        (image_rows[i] >> (BITS_X_LED * (SIZE - 1)));
    }
}

// Rotates columns left
static void move_left(image_t *image)
{
    uint32_t *image_rows = image -> rows;

    for (int i = 0; i < SIZE; i++)
    {
        // Execute right rotation
        image_rows[i] = (image_rows[i] >> BITS_X_LED) | 
                        (image_rows[i] << (BITS_X_LED * (SIZE - 1))); 
    }
}

// Store to gallery
void store()
{
    initialised = false;
    head_frame  = add_frame(head_frame, HALT);
    head_frame  = head_frame -> prev;
    (video_gallery -> items_to_display)++;
    video_gallery = add_video(video_gallery, original_image, head_frame);
}
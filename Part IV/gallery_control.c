#include "gallery_control.h"
/*--------------------------*//* MAKE PROCEDURES *//*-------------------------*/
gallery_t *make_gallery() 
{
    gallery_t *gallery = malloc(sizeof(gallery_t));
    
    if (gallery == NULL)
        perror("Could not make gallery");
    
    gallery -> head = NULL;
    gallery -> items_to_display = 0;

    return gallery;
}

frame_node_t *make_frame()
{
    frame_node_t *frame = malloc(sizeof(frame_node_t));
    
    if (frame == NULL)
        perror("Could not make frame");
    
    frame -> next = NULL;
    frame -> prev = NULL;

    return frame;
}

gallery_node_t *make_node()
{
    gallery_node_t *node = malloc(sizeof(gallery_node_t));
    if(node == NULL)
        perror("Could not make node");

    return node;
}

/*--------------------------*//* ADD PROCEDURES *//*--------------------------*/
// Make a new gallery node and enquqe it, by putting it at the end of the 
// gallery. Same for frames.
gallery_t *add_video(gallery_t *gallery, 
                     image_t *new_image, 
                     frame_node_t *frames)
{

    gallery_node_t *new_node = make_node();

    new_node -> initial_image = new_image;
    new_node -> first_frame   = frames;
    
    if (gallery -> head == NULL)
    {
        gallery -> head = new_node;
        gallery -> head -> prev = new_node;
        gallery -> head -> next = new_node;
    } else {
        new_node -> next = gallery -> head;
        new_node -> prev = gallery -> head -> prev;

        gallery -> head -> prev -> next = new_node;
        gallery -> head -> prev         = new_node;
    }

    return gallery;
}

frame_node_t *add_frame(frame_node_t *frames, move_t next_move)
{
    frame_node_t *new_frame = make_frame();

    new_frame -> move = next_move;
    
    if (frames -> next == NULL && frames -> prev == NULL)
    {
        frames = new_frame;

        frames -> next = frames;
        frames -> prev = frames;
    } else {
        new_frame -> next = frames;
        new_frame -> prev = frames -> prev;

        frames -> prev -> next = new_frame;
        frames -> prev         = new_frame;
    }

    return frames;
}
/*-------------------------*//* ITERATE PROCEDURES *//*-----------------------*/

gallery_iterator gallery_next(gallery_iterator iterator) 
{
    return iterator -> next;
}


gallery_iterator gallery_previous(gallery_iterator iterator) 
{
    return iterator -> prev;
}

frame_iterator next_frame(frame_iterator iterator)
{
    return iterator -> next;
}


frame_iterator previous_frame(frame_iterator iterator)
{
    return iterator -> prev;
}
/*-------------------------*//* REMOVE PROCEDURES *//*------------------------*/

gallery_t *remove_last_entry(gallery_t *gallery)
{
    if (gallery -> head -> next != gallery -> head)
    {
        gallery_node_t *to_remove = gallery -> head -> prev;
        to_remove -> prev -> next = gallery -> head;
        gallery -> head -> prev   = to_remove -> prev; 
    } 
    else
    {
        gallery -> head = NULL;
    }

    return gallery;   
}

/*--------------------------*//* FREE PROCEDURES *//*-------------------------*/
void destroy_gallery(gallery_t *gallery)
{   
    gallery_iterator iterator = gallery -> head;
    while(iterator != gallery -> head -> prev)
    {
        gallery_node_t *temp = iterator;
        iterator = gallery_next(iterator);
        destroy_gallery_node(temp);
    }
    destroy_gallery_node(gallery -> head);
    
    free(gallery);
}

static void destroy_gallery_node(gallery_node_t *node)
{   
    free(node -> initial_image);
    destroy_frames(node -> first_frame);
    free(node);
}

static void destroy_frames(frame_node_t *frame)
{
    frame_iterator iterator = frame;
    while(iterator != frame -> prev)
    {
        frame_node_t *temp = iterator;
        iterator = next_frame(iterator);
        free(temp);
    }
    free(frame);
}

/*----------------------------------------------------------------------------*/
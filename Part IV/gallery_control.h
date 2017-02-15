/*-------------------------------*//* STRUCTURES *//*-------------------------*/
// The gallery has a circular doubly linked list of gallery nodes, each of which
// has the first frame of the animation and a pointer to the head of a linked 
// list of video nodes which contain the direction in which the initial image
// will move.  
typedef struct frame_node
{
    struct frame_node *next;
    struct frame_node *prev;
    move_t move;
} frame_node_t;

typedef struct gallery_node
{
  struct gallery_node *next;
  struct gallery_node *prev;
  frame_node_t *first_frame;
  image_t* initial_image;

} gallery_node_t;

typedef struct gallery {
    gallery_node_t *head;
    int items_to_display;
} gallery_t;

typedef gallery_node_t *gallery_iterator;
typedef frame_node_t     *frame_iterator;
/*-------------------------*//* FUNCTION PROTOTYPES *//*----------------------*/
gallery_t   *make_gallery();
frame_node_t  *make_frame();
gallery_node_t *make_node();
gallery_t *add_video(gallery_t*, image_t*, frame_node_t*);
frame_node_t *add_frame(frame_node_t*, move_t);
gallery_iterator gallery_next(gallery_iterator);
gallery_iterator gallery_previous(gallery_iterator);
frame_iterator next_frame(frame_iterator);
frame_iterator previous_frame(frame_iterator);
gallery_t *remove_last_entry(gallery_t*);
void destroy_gallery(gallery_t*);
static void destroy_gallery_node(gallery_node_t*);
static void destroy_frames(frame_node_t*);

/*----------------------------------------------------------------------------*/

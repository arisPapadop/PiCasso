#include "gallery.h"

extern gallery_t *video_gallery;

bool ready_to_play = false;
bool next_video    = true;

frame_iterator frame;
gallery_node_t *current_video;

void gallery(image_t *image)
{	
	if( video_gallery -> items_to_display > 0)
	{
		if (!ready_to_play)
		{
			ready_to_play = true;
			current_video = video_gallery -> head;
			frame = current_video -> first_frame;
		}

		// Button detection
		if (digitalRead(RIGHT_BTN)) 
		{
			if (next_video) 
			{
				current_video = gallery_next(current_video);
				frame = current_video -> first_frame;
				next_video = false;
			}
		}
		else if (digitalRead(LEFT_BTN))
		{
			if (next_video) 
			{
				current_video = gallery_previous(current_video);
				frame = current_video -> first_frame;
				next_video = false;
			}
		}
		else
		{
			next_video = true;
		}

		if (frame -> move == HALT)
		{
			// We need to detect when we have looped through all the frames in
			// order to reset the image back to the original.
			*image = *(current_video -> initial_image);
		}
		else
		{
			// Otherwise, we just keep moving normally.
			move_internal(image, frame -> move);
		}
		frame = next_frame(frame);
	}
}
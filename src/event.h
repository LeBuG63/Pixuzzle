#pragma once

#include <SDL\SDL.h>

typedef struct event_s {
	unsigned char	key[SDLK_LAST];
	unsigned int	close_window  : 1;
	unsigned int	mouse_clicked : 1;
	int				mouse_x;
	int				mouse_y;

}Event;

void	event_init(Event *event);
void	event_update(Event *event);

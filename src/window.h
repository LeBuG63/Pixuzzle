#pragma once

#include <SDL\SDL.h>

typedef struct {
	SDL_Surface *screen;

	int			width;
	int			height;
}	Window;

Window	*window_create(int width, int height, const char *title);
void	window_destroy(Window *win);

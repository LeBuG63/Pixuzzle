#pragma once

#include <SDL\SDL.h>

typedef struct timer_s {
	double		time;
	char		time_char[16];
	SDL_Surface *time_print;
	SDL_Rect	rect;
	SDL_Color	color;
}	Timer;
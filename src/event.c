#include "event.h"

void	event_init(Event *event) {
	int		i;

	for (i = 0; i < SDLK_LAST; ++i)
		event->key[i] = 0;

	event->close_window  = 0;
	event->mouse_clicked = 0;

	event->mouse_x = 0;
	event->mouse_y = 0;
}

void	event_update(Event *event) {
	SDL_Event	sevent;

	while (SDL_PollEvent(&sevent)) {
		switch (sevent.type) {
		case SDL_QUIT:
		case SDLK_ESCAPE:
			event->close_window = 1;
			break;
		case SDL_KEYUP:
			event->key[sevent.key.keysym.sym] = 0;
			break;
		case SDL_KEYDOWN:
			event->key[sevent.key.keysym.sym] = 1;
			break;
		case SDL_MOUSEMOTION:
			event->mouse_x = sevent.motion.x;
			event->mouse_y = sevent.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			event->mouse_clicked = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			event->mouse_clicked = 0;
			break;
		}
	}
}

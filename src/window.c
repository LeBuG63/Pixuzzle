#include "window.h"

#include <SDL\SDL_ttf.h>

Window	*window_create(int width, int height, const char *title) {
	Window	*win = malloc(sizeof(Window));

	if (!win)
		return NULL;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	win->screen = SDL_SetVideoMode(width, height, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_DOUBLEBUF);

	win->width = width;
	win->height = height;

	SDL_WM_SetCaption(title, NULL);
	SDL_WM_SetIcon(IMG_Load("res/img/icone_1.jpg"), NULL);

	return win;
}

void	window_destroy(Window *win) {
	SDL_FreeSurface(win->screen);
	free(win);

	TTF_Quit();
	SDL_Quit();
}

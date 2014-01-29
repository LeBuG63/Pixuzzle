#include <stdio.h>
#include <time.h>

#include "window.h"
#include "menu.h"
#include "game.h"

#pragma comment (lib, "SDL.lib")
#pragma comment (lib, "SDLmain.lib")
#pragma comment (lib, "SDL_image.lib")
#pragma comment (lib, "SDL_ttf.lib")

int main(int argc, char *argv) {

	Window	*win;

	win = window_create(720, 720, "Pixuzzle");

	if (!win)
		return 1;

	srand(time(NULL));
	
	splash_screen(win);
	main_menu(win);

	window_destroy(win);

	return 0;
}

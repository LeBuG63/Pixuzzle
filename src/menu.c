#include "menu.h"
#include "event.h"
#include "map.h"
#include "engine.h"
#include "game.h"

#include <time.h>
#include <string.h>

#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>

void	splash_screen(Window *win) {
	Event		event;
	int			timer;
	int			i = 0;
	SDL_Surface	*surface = IMG_Load("res/img/splashscreen.png");
	SDL_Surface *deg = SDL_CreateRGBSurface(SDL_HWSURFACE, win->width, win->height, 32, 0, 0, 0, 0);

	event_init(&event);
	SDL_SetAlpha(deg, SDL_SRCALPHA, 0);

	while (!event.close_window) {

		event_update(&event);

		if ((timer = SDL_GetTicks()) >= 3 * CLOCKS_PER_SEC) {
			if (i >= 255)
				goto stop;
			SDL_SetAlpha(deg, SDL_SRCALPHA, i += 1);
		}

		SDL_FillRect(win->screen, NULL, 0x000000);

		SDL_BlitSurface(surface, NULL, win->screen, NULL);
		SDL_BlitSurface(deg, NULL, win->screen, NULL);

		SDL_Flip(win->screen);
	}

stop:
	SDL_FreeSurface(surface);
	SDL_FreeSurface(deg);
}

void	get_hight_score(double *time, FILE *file) {
	if (file) {
		fscanf(file, "%lf %lf %lf", &time[0], &time[1], &time[2]);
		rewind(file);
	}
	else {
		time[0] = 0.0;
		time[1] = 0.0;
		time[2] = 0.0;
	}
}

void	main_menu(Window *win) {
	Event			event;
	map_t			map;

	int				velocity = 2;

	unsigned int	difficulty = 0;
	unsigned int	last = 0;
	unsigned int	blit_time = 1;

	double			time[3] = { 0 };
	char			best_time_char[10] = { ' ' };

	TTF_Font		*font = TTF_OpenFont("res/fonts/Squareo.ttf", 72);
	SDL_Surface		*text = NULL, *best_time = NULL;
	SDL_Surface		*srfc_difficulty = NULL;
	SDL_Color		color = { 255, 255, 255 };
	SDL_Rect		rect, rect_diff, rect_best_time;

	FILE			*file = fopen("res/save/save.px", "r");

	if (!font)
		exit(0);

	event_init(&event);

	map.ncell_height = 30;
	map.ncell_width = 30;

	map_init(&map, win);

	text = TTF_RenderText_Blended(font, "Pixuzzle", color);

	font = TTF_OpenFont("res/fonts/imagine_font.ttf", 22);

	get_hight_score(time, file);
	sprintf(best_time_char, "%.03lf", time[difficulty]);
	best_time = TTF_RenderText_Blended(font, best_time_char, color);


	srfc_difficulty = IMG_Load("res/img/difficulty.png");

	rect.x = (win->width >> 1) - ((strlen("Pixuzzle") * 72) >> 1);
	rect.y = 32;

	rect_diff.x = 0;
	rect_diff.y = -((win->height >> 1) - (260 >> 1));
	rect_diff.h = 720;
	rect_diff.w = 720 * 3;

	rect_best_time.x = 280;
	rect_best_time.y = 413;

	_game_over = 0;

	while (!event.close_window) {

		if (engine_need_update(FPS)){
			event_update(&event);

			if (event.key[SDLK_LEFT]) {
				if (difficulty > 0) {
					--difficulty;
					last = 2;
					event.key[SDLK_LEFT] = 0;
					sprintf(best_time_char, "%.03lf", time[difficulty]);
					best_time = TTF_RenderText_Blended(font, best_time_char, color);
				}
			}

			if (event.key[SDLK_RIGHT]) {
				if (difficulty < 2) {
					++difficulty;
					last = 1;
					event.key[SDLK_RIGHT] = 0;
					sprintf(best_time_char, "%.03lf", time[difficulty]);
					best_time = TTF_RenderText_Blended(font, best_time_char, color);
				}
			}

			if (event.key[SDLK_RETURN]) {
				map_free(&map);

				event.key[SDLK_RETURN] = 0;
				game_run(win, difficulty);

				get_hight_score(time, file);
				sprintf(best_time_char, "%.03lf", time[difficulty]);
				best_time = TTF_RenderText_Blended(font, best_time_char, color);

				map.ncell_height = map.ncell_width = ((rand() % 4) + 1) * 10;

				map_init(&map, win);
			}

			else {
				if (rect_diff.x > (difficulty * 720) && last == 2) {
					rect_diff.x -= 40;
					blit_time = 0;
				}
				else if (rect_diff.x < (difficulty * 720) && last == 1) {
					rect_diff.x += 40;
					blit_time = 0;
				}
				else {
					blit_time = 1;
				}

				engine_move_tile(&map);

				if (_game_over) {
					map_fill(&map);
					_game_over = 0;
				}

				engine_add_cell(&map, NEW_CELL_DELAY);

				if (rect.x <= 0)
					velocity = 2;
				else if (rect.x + text->w >= win->width)
					velocity = -2;

				rect.x += velocity;

				SDL_FillRect(win->screen, NULL, 0x000000);

				map_blit(map, win);
				SDL_BlitSurface(text, NULL, win->screen, &rect);
				SDL_BlitSurface(srfc_difficulty, &rect_diff, win->screen, NULL);

				if (blit_time)
					SDL_BlitSurface(best_time, NULL, win->screen, &rect_best_time);

				SDL_Flip(win->screen);
			}
		}
	}
	SDL_FreeSurface(text);
	SDL_FreeSurface(srfc_difficulty);
	TTF_CloseFont(font);


	fclose(file);
	map_free(&map);
}

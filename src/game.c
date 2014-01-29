#include "game.h"

#include "map.h"
#include "engine.h"
#include "comp.h"

#include <stdio.h>

static void	set_difficulty(map_t *map, unsigned int difficulty) {
	switch (difficulty) {
	case 0:
		map->ncell_height = map->ncell_width = 40;
		break;
	case 1:
		map->ncell_height = map->ncell_width = 20;
		break;
	case 2:
		map->ncell_height = map->ncell_width = 10;
		break;
	}
}

static void print_crono(Window *win, TTF_Font *font, Timer *timer) {
	if (timer->time_print != NULL)
		SDL_FreeSurface(timer->time_print);

	sprintf(timer->time_char, "%.3lf", (timer->time += 1000.0/FPS) / 1000.0);
	timer->time_print = TTF_RenderText_Blended(font, timer->time_char, timer->color);

	SDL_BlitSurface(timer->time_print, NULL, win->screen, &timer->rect);
}

void	game_run(Window *win, unsigned int difficulty) {
	Event			event;
	map_t			map;
	TTF_Font		*font = TTF_OpenFont("res/fonts/imagine_font.ttf", 32);
	Timer			timer;

	timer.rect.x = 300;
	timer.rect.y = 50;
	timer.time = 0.0;
	timer.color.r = timer.color.g = timer.color.b = 255;

	timer.time_print = TTF_RenderText_Blended(font, timer.time_char, timer.color);
	
	event_init(&event);

	set_difficulty(&map, difficulty);
	
	_game_over = 0;

	map_init(&map, win);

	while (!event.close_window) {
		
		if (engine_need_update(FPS)) {
			event_update(&event);

			engine_move_tile(&map);
			engine_add_cell(&map, NEW_CELL_DELAY);

			if (event.mouse_clicked) {
				blast_cell_mouse(&map, event.mouse_x, event.mouse_y);
				event.mouse_clicked = 0;
			}

			if (_game_over) {
				game_over(win, font, event, timer.time);
				game_save_score(timer.time / 1000.0, difficulty);
				event.close_window = 1;
			}
			else {
				SDL_FillRect(win->screen, NULL, 0x000000);

				map_blit(map, win); 
				print_crono(win, font, &timer);

				while(SDL_Flip(win->screen) == -1)
					SDL_Delay(1);
			}
		}
	}
	TTF_CloseFont(font);
	SDL_FreeSurface(timer.time_print);
	map_free(&map);
}

void	game_over(Window *win, TTF_Font *font, Event event, double time) {
	SDL_Surface *text = NULL;
	SDL_Rect	rect;
	char		time_char[50];
	SDL_Color	color = { 200, 200, 200 };

	sprintf(time_char, "Time: %.3lf seconds !", time / 1000.0);

	text = TTF_RenderText_Blended(font, time_char, color);

	rect.x = (win->width >> 1) - ((strlen(time_char) << 4) >> 1);
	rect.y = (win->width >> 1) - 32;

	while (!event.close_window) {
		event_update(&event);

		if (event.key[SDLK_SPACE] || event.key[SDLK_RETURN] || event.mouse_clicked) {
			event.close_window = 1;
		}

		SDL_FillRect(win->screen, NULL, 0x000000);

		SDL_BlitSurface(text, NULL, win->screen, &rect);

		SDL_Flip(win->screen);		
	}

	SDL_FreeSurface(text);
}

void	game_save_score(double time, unsigned int difficulty) {
	FILE	*file = fopen("res/save/save.px", "r+");
	double	diff[3] = { 0 };
	int		i = 0;

	if (file) {
		rewind(file);

		fscanf(file, "%lf", &diff[0]);
		fscanf(file, "%lf", &diff[1]);
		fscanf(file, "%lf", &diff[2]);

		rewind(file);
				
		if (difficulty == 0 && cmpDoubles(diff[0], time) == 1)
			fprintf(file, "%lf ", time);
		else
			fprintf(file, "%lf ", diff[0]);

		if (difficulty == 1 && cmpDoubles(diff[1], time) == 1)
			fprintf(file, "%lf ", time);
		else
			fprintf(file, "%lf ", diff[1]);

		if (difficulty == 2 && cmpDoubles(diff[2], time) == 1)
			fprintf(file, "%lf ", time);
		else
			fprintf(file, "%lf ", diff[2]);
	}
	fclose(file);
}

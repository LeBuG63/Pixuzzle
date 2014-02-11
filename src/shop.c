#include "shop.h"
#include "event.h"
#include "engine.h"
#include "event.h"
#include "save.h"

#include <SDL\SDL_image.h>

static SDL_Surface *set_text_items(TTF_Font *font, int cell_type) {
	static SDL_Color	color_fg_unlocked = { 0, 255, 0 }, color_fg_locked = { 255, 0, 0 };
	static SDL_Color	color_bg = { 0, 0, 0 };

	SDL_Surface			*surface = NULL;

	if (cell_type == CELL_UNLOCKED)
		surface = TTF_RenderText_Shaded(font, "Unlocked", color_fg_unlocked, color_bg);
	else
		surface = TTF_RenderText_Shaded(font, "Locked", color_fg_locked, color_bg);

	return surface;
}

static void	blit_text_items(TTF_Font *font, cell_type_t cell_type, Window *win) {
	SDL_Surface	*is_locked = NULL;
	SDL_Rect	rect;
	SDL_Color	color_fg_unlocked = { 0, 255, 0 }, color_fg_locked = { 255, 0, 0 };
	SDL_Color	color_bg = { 0, 0, 0 };

	rect.x = 177;
	
	is_locked = set_text_items(font, cell_type.cell_white);
	rect.y = 130;
	SDL_BlitSurface(is_locked, NULL, win->screen, &rect); SDL_FreeSurface(is_locked);

	is_locked = set_text_items(font, cell_type.cell_lightgreen);
	rect.y = 265;
	SDL_BlitSurface(is_locked, NULL, win->screen, &rect); SDL_FreeSurface(is_locked);

	is_locked = set_text_items(font, cell_type.cell_lightblue);
	rect.y = 400;
	SDL_BlitSurface(is_locked, NULL, win->screen, &rect); SDL_FreeSurface(is_locked);

	is_locked = set_text_items(font, cell_type.cell_lightred);
	rect.y = 532;
	SDL_BlitSurface(is_locked, NULL, win->screen, &rect); SDL_FreeSurface(is_locked);

	is_locked = set_text_items(font, cell_type.cell_darkblue);
	rect.y = 666;
	SDL_BlitSurface(is_locked, NULL, win->screen, &rect); SDL_FreeSurface(is_locked);

}

static void	buy_item(Window *win, cell_type_t *cell_type, SDL_Surface **item, Event *event, int *pixels) {
	static SDL_Rect	rect = { .x = 655, .y = 26 };
	static int item_n = 0;

	if (event->key[SDLK_UP]) {
		--item_n;
		event->key[SDLK_UP] = 0;
		if (item_n < 0) {
			item_n = 0;
		}
	}
	if (event->key[SDLK_DOWN]) {
		++item_n;
		event->key[SDLK_DOWN] = 0;
		if (item_n > NCELL_TYPE - 5 - 1) {
			item_n = NCELL_TYPE - 5 - 1;
		}
	}
	if (event->key[SDLK_RETURN] || event->key[SDLK_SPACE]) {
		event->key[SDLK_RETURN] = 0;
		event->key[SDLK_SPACE] = 0;

		if ((*pixels - PRICE[item_n]) > 0) {
			switch (item_n) {
			case 0:
				if (!cell_type->cell_white) {
					cell_type->cell_white = CELL_UNLOCKED;
					*pixels -= PRICE[item_n];
				}
				break;
			case 1:
				if (!cell_type->cell_lightgreen) {
					cell_type->cell_lightgreen = CELL_UNLOCKED;
					*pixels -= PRICE[item_n];
				}
				break;
			case 2:
				if (!cell_type->cell_lightblue) {
					cell_type->cell_lightblue = CELL_UNLOCKED;
					*pixels -= PRICE[item_n];
				}
				break;
			case 3:
				if (!cell_type->cell_lightred) {
					cell_type->cell_lightred = CELL_UNLOCKED;
					*pixels -= PRICE[item_n];
				}
				break;
			case 4:
				if (!cell_type->cell_darkblue) {
					cell_type->cell_darkblue = CELL_UNLOCKED;
					*pixels -= PRICE[item_n];
				}
				break;
			}
		}
	}

	SDL_BlitSurface(item[item_n], NULL, win->screen, &rect);
}


static void		create_items_icons(SDL_Surface **item) {
	int i;

	for (i = 0; i < NCELL_TYPE - 5; ++i) {
		item[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, 33, 33, 32, 0, 0, 0, 0);
		item[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, 33, 33, 32, 0, 0, 0, 0);
		item[2] = SDL_CreateRGBSurface(SDL_HWSURFACE, 33, 33, 32, 0, 0, 0, 0);
		item[3] = SDL_CreateRGBSurface(SDL_HWSURFACE, 33, 33, 32, 0, 0, 0, 0);
		item[4] = SDL_CreateRGBSurface(SDL_HWSURFACE, 33, 33, 32, 0, 0, 0, 0);
	}

	SDL_FillRect(item[0], NULL, COLOR_WHITE);
	SDL_FillRect(item[1], NULL, COLOR_LIGHTGREEN);
	SDL_FillRect(item[2], NULL, COLOR_LIGHTBLUE);
	SDL_FillRect(item[3], NULL, COLOR_LIGHTRED);
	SDL_FillRect(item[4], NULL, COLOR_DARKGREY);
}

void	shop(Window *win) {
	int				pixels, old_pixels;
	char			pixel_char[50];
	int				i;
	Event			event;

	SDL_Surface		*item[5] = { { NULL } };
	SDL_Surface		*background = NULL;
	SDL_Surface		*spixel = NULL;

	SDL_Color		color = { 255, 255, 255 };
	SDL_Rect		rect_pixel;
	TTF_Font		*font = TTF_OpenFont("res/fonts/imagine_font.ttf", 24);
	cell_type_t		cell_type;

	background = IMG_Load("res/img/shop.png");

	create_items_icons(item);

	pixels = save_get_pixels();
	save_get_game_cell_unlock(&cell_type);

	event_init(&event);

	sprintf(pixel_char, "Pixels: %u", pixels);
	spixel = TTF_RenderText_Blended(font, pixel_char, color);
	rect_pixel.x = 300;
	rect_pixel.y = 78;

	font = TTF_OpenFont("res/fonts/imagine_font.ttf", 32);

	while (!event.close_window) {
		if (engine_need_update(FPS)) {
			event_update(&event);

			if (event.key[SDLK_ESCAPE])
				event.close_window = 1;

			old_pixels = pixels;

			SDL_FillRect(win->screen, NULL, 0x000000);

			SDL_BlitSurface(background, NULL, win->screen, NULL);
			blit_text_items(font, cell_type, win);

			buy_item(win, &cell_type, item, &event, &pixels);
			
			if (old_pixels != pixels) {
				font = TTF_OpenFont("res/fonts/imagine_font.ttf", 24);

				sprintf(pixel_char, "Pixels: %u", pixels);
				SDL_FreeSurface(spixel);
				spixel = TTF_RenderText_Blended(font, pixel_char, color);

				font = TTF_OpenFont("res/fonts/imagine_font.ttf", 32);
			}

			SDL_BlitSurface(spixel, NULL, win->screen, &rect_pixel);

			while (SDL_Flip(win->screen) == -1) {
				SDL_Delay(1);
			}
		}
	}

	TTF_CloseFont(font);
	SDL_FreeSurface(background);

	for (i = 0; i < NCELL_TYPE - 5; ++i) {
		SDL_FreeSurface(item[i]);
	}

	save_cell_pixels_unlocked(cell_type, pixels);
}

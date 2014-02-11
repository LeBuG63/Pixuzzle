#pragma once

#include "window.h"
#include "cell_type.h"
#include "event.h"

#include <SDL\SDL_ttf.h>
#include <SDL\SDL.h>

static const int PRICE[] = {
	500,
	1000,
	1500,
	2500,
	5000
};

static SDL_Surface *set_text_items(TTF_Font *font, int cell_type);
static void	blit_text_items(TTF_Font *font, cell_type_t cell_type, Window *win);
static void	buy_item(Window *win, cell_type_t *cell_type, SDL_Surface **item, Event *event, int *pixels);
static void		create_items_icons(SDL_Surface **item);

void	shop(Window *win);

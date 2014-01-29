#pragma once

#include "window.h"

#define COLOR_BLACK		0x000000
#define COLOR_RED		0xFF0000
#define COLOR_GREEN		0x00FF00
#define COLOR_BLUE		0x0000FF
#define COLOR_PUPRLE	0xFF00FF
#define COLOR_WHITE		0xFFFFFF

typedef struct map_s {
	unsigned int	**arr;
	unsigned int	**arr_bool;
	int				cell_height;
	int				cell_width;
	int				ncell_width;
	int				ncell_height;
	SDL_Surface		*surface;
}map_t;

void	map_init(map_t *map, Window *win);
void	map_free(map_t *map);

void	map_fill(map_t *map);

void	map_add_cell(map_t *map, int x, int y, unsigned int color);
void	map_del_cell(map_t *map, int x, int y);

void	map_blit(map_t map, Window *win);

unsigned int	map_cell_is_empty(map_t map, int x, int y);
unsigned int	map_get_cell(map_t map, int x, int y);

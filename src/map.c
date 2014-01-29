#include "map.h"
#include "window.h"

#include <stdlib.h>

void	map_init(map_t *map, Window	 *win) {
	int x, y;

	map->cell_height = (int)win->height / map->ncell_height;
	map->cell_width = (int)win->height / map->ncell_width;

	map->arr = malloc(map->ncell_height * sizeof(unsigned int *));
	map->arr_bool = malloc(map->ncell_height * sizeof(unsigned int *));

	for (x = 0; x < map->ncell_height; ++x) {
		map->arr[x] = malloc(map->ncell_width * sizeof(unsigned int **));
		map->arr_bool[x] = malloc(map->ncell_width * sizeof(unsigned int **));
	}

	for (x = 0; x < map->ncell_width; ++x)
		for (y = 0; y < map->ncell_height; ++y) {
			map->arr[x][y] = COLOR_BLACK;
			map->arr_bool[x][y] = 0;
	}
	
	map->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, map->cell_width, map->cell_height, 32, 0, 0, 0, 0);
}

void	map_free(map_t *map) {
	int i;

	for (i = 0; i < map->ncell_height; ++i) {
		free(map->arr[i]);
		free(map->arr_bool[i]);
	}

	free(map->arr);
	free(map->arr_bool);

	map->arr = NULL;
	map->arr_bool = NULL;

	SDL_FreeSurface(map->surface);
}

void	map_fill(map_t *map) {
	int x, y;

	for (x = 0; x < map->ncell_width; ++x)
		for (y = 0; y < map->ncell_height; ++y)
			map->arr[x][y] = 0;
}

void	map_add_cell(map_t *map, int x, int y, unsigned int color) {
	map->arr[x][y] = color;
}

void	map_del_cell(map_t *map, int x, int y) {
	map->arr[x][y] = COLOR_BLACK;
}

void	map_blit(map_t map, Window *win) {
	int			x, y;
	SDL_Rect	rect;

	for (x = 0; x < map.ncell_width; ++x) {
		for (y = 0; y < map.ncell_height; ++y) {

			rect.x = x * map.cell_width;
			rect.y = y * map.cell_height;

			SDL_FillRect(map.surface, NULL, map_get_cell(map, x, y));

			SDL_BlitSurface(map.surface, NULL, win->screen, &rect);
		}
	}
}

unsigned int	map_cell_is_empty(map_t map, int x, int y) {
	return map.arr[x][y] != COLOR_BLACK;
}

unsigned int	map_get_cell(map_t map, int x, int y) {
	return map.arr[x][y];
}

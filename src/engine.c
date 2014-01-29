#include "engine.h"
#include "game.h"

#include <time.h>


int		engine_need_update(int fps) {
	static unsigned int	start_ticks = 0, ticks;

	ticks = SDL_GetTicks();

	if (ticks - start_ticks < 1000 / FPS)
		return 0;

	start_ticks = ticks;
	return 1;
}

void	engine_move_tile(map_t *map) {
	int x, y;

	unsigned int	current_cell, below_cell, current_cell_color;

	for (x = 0; x < map->ncell_width; ++x) {
		for (y = 0; y < map->ncell_height; ++y) {
			map->arr_bool[x][y] = 0;
		}
	}

	for (x = 0; x < map->ncell_width; ++x) {
		for (y = 0; y < map->ncell_height; ++y) {
			below_cell = map_get_cell(*map,x, y + 1);
			current_cell = map->arr_bool[x][y];
			current_cell_color	= map_get_cell(*map, x, y);

			if (current_cell == 0 && below_cell == 0 
				&& y < (map->ncell_height - 1) * map->cell_height) {
				
				map_del_cell(map, x, y);
				map_add_cell(map, x, y + 1, current_cell_color);

				map->arr_bool[x][y + 1] = 1;
				map->arr_bool[x][y] = 0;
			}

			if (map_get_cell(*map, x, 0) != COLOR_BLACK && map_get_cell(*map, x, 1) != COLOR_BLACK && map_get_cell(*map, x, 2) != COLOR_BLACK)
				_game_over = 1;
		}
	}
}

void	engine_add_cell_rand(map_t *map) {
	int			 x,
				 chance_white;
	unsigned int color;

	x = rand() % map->ncell_width;

	color = rand() % 5;

	switch (color) {
	case 1:
		color = COLOR_BLUE;
		break;
	case 2:
		color = COLOR_RED;
		break;
	case 3:
		color = COLOR_GREEN;
		break;
	case 4:
		color = COLOR_PUPRLE;
		break;
	default:
		color = COLOR_BLACK;
	}

	chance_white = rand() % 276;

	if (chance_white == 98)
		color = COLOR_WHITE;

	map_add_cell(map, x, 0, color);
}

void	engine_add_cell(map_t *map, int delay) {
	static unsigned char new_cell_delay = 0;

	if (++new_cell_delay > (unsigned char)delay) {
		engine_add_cell_rand(map);
		new_cell_delay = 0;
	}
}

void	blast_cell_mouse(map_t *map, int mouse_x, int mouse_y) {
	int hit_cell_row, hit_cell_col;

	hit_cell_row = mouse_x / map->cell_width;
	hit_cell_col = mouse_y / map->cell_height;

	init_blast_cell(map, hit_cell_row, hit_cell_col);
}

void	init_blast_cell(map_t *map, int row, int col) {
	unsigned int color = map_get_cell(*map, row, col);

	if (color == COLOR_WHITE) {
		int x, y;

		map_add_cell(map, row, col, COLOR_BLACK);

		for (x = 0; x < map->ncell_width; ++x)
			for (y = (col > ((map->ncell_height - 5)) ? map->ncell_height + 5 : col); y < col + 5; ++y)
				map_del_cell(map, x, y);
	}
	else if (color != COLOR_BLACK) {
		if (row > 0 && map_get_cell(*map, row - 1, col) == color
			|| col > 0 && map_get_cell(*map, row, col - 1) == color
			|| row < map->ncell_width - 1 && map_get_cell(*map, row + 1, col) == color
			|| col < map->ncell_height - 1 && map_get_cell(*map, row, col + 1) == color) {
			map_del_cell(map, row, col);
			rec_cell_blast(map, row, col, color);
		}
		else {
			int i;

			for (i = 0; i < 20; ++i) {
				engine_add_cell_rand(map);
			}
		}
	}
	else {
		int i;
		for (i = 0; i < 20; ++i)
			engine_add_cell_rand(map);
	}
}

void	blast_cell(map_t *map, int row, int col) {
	unsigned int color = map_get_cell(*map, row, col);

	if (color != COLOR_BLACK) {
		map_del_cell(map, row, col);
		rec_cell_blast(map, row, col, color);
	}
}

void	rec_cell_blast(map_t *map, int row, int col, unsigned int color) {
	if (row > 0 && map_get_cell(*map, row - 1, col) == color)
		blast_cell(map, row - 1, col);

	if (col > 0 && map_get_cell(*map, row, col - 1) == color)
		blast_cell(map, row, col - 1);

	if (row < map->ncell_width-1 && map_get_cell(*map, row + 1, col) == color)
		blast_cell(map, row + 1, col);

	if (col < map->ncell_height-1 && map_get_cell(*map, row, col + 1) == color)
		blast_cell(map, row, col + 1);
}

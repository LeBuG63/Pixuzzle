#pragma once

#include "map.h"

int		engine_need_update(int fps);
void	engine_move_tile(map_t *map);
void	engine_add_cell_rand(map_t *map);
void	engine_add_cell(map_t *map, int delay);

void	blast_cell_mouse(map_t *map, int mouse_x, int mouse_y);
void	init_blast_cell(map_t *map, int row, int col);
void	blast_cell(map_t *map, int row, int col);
void	rec_cell_blast(map_t *map, int row, int col, unsigned int color);
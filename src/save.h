#pragma once

#include "cell_type.h"

void	save_open(void);
void	save_close(void);

void	save_game_score(double time, unsigned int difficulty);
void	save_get_game_score(double *time);
void	save_get_game_cell_unlock(cell_type_t *cell_type);

void	save_game_cell_unlock(cell_type_t cell_type);
void	save_cell_pixels_unlocked(cell_type_t cell_type, int pixels);
void	save_pixels(int pixels);

int		save_get_pixels(void);

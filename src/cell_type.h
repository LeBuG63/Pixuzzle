#pragma once

#define CELL_LOCKED		0
#define CELL_UNLOCKED	1

#define NCELL_TYPE		9

typedef struct cell_s {
	unsigned int	cell_red		: 1;
	unsigned int	cell_green		: 1;
	unsigned int	cell_blue		: 1;
	unsigned int	cell_white		: 1;
	unsigned int	cell_darkblue	: 1;
	unsigned int	cell_lightgreen : 1;
	unsigned int	cell_lightblue  : 1;
	unsigned int	cell_lightred	: 1;
}	cell_type_t;

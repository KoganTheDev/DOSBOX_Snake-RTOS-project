// This header and C file are responsible on creating the food object and respawn it when needed
#pragma once

#ifndef FOOD_H
#define FOOD_H

#include "game_m~1.h"

typedef enum limits_for_spawning_elements
{
	UPPER_LIMIT = 2, // One row lower than the northern boundry
	LOWER_LIMIT = DISPLAY_ROWS - 2, // One row higher than the southern boundry
	LEFT_LIMIT = 1, // One row to the right of the left boundry
	RIGHT_LIMIT = DISPLAY_COLS - 2, // One row to the left of the right limit
};

extern POSITION food; // Position of the food
extern int food_eaten;

void spawn_food(); // Spawns food at a random position not occupied by the snake.
void draw_food(); // Inserts the food object into the display draft
int is_snake_on_food(); // Checks if the snake on the food, if so enlarge the snake by one

#endif

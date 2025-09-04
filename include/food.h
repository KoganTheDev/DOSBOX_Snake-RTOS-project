#pragma once

#ifndef FOOD_H
#define FOOD_H

#include "game_m~1.h"

// TODO: MAX SIZE FOR FOOD IS 3x3, note that WALL max size is also a 3x3 but diagnally
//! TEMP NAME: this enum holds the bounds in which elements can be spawned in
//!		- the wall object will probably another copy of this one
//!		- for refactoring, will be smarter to put the enum under game_management.h and include "game_m~1.h"
typedef enum limits_for_spawning_elements
{
	//! NOTE: CURRENT LIMIT IS SET FOR ELEMENTS OF SIZE 1x1, SIZE 2x2 or more will probably cause bugs!

	//! LIMITS ARE INCLUSIVE meaning an object can be spawned on 
	UPPER_LIMIT = 3, // One row lower than the northern boundry
	LOWER_LIMIT = DISPLAY_ROWS - 2, // One row higher than the southern boundry
	LEFT_LIMIT = 1, // One row to the right of the left boundry
	RIGHT_LIMIT = DISPLAY_COLS - 2, // One row to the left of the right limit
};


extern POSITION food; // Position of the food

void spawn_food(); // Spawns food at a random position not occupied by the snake.
int is_snake_on_food(); // Checks if the snake on the food, if so enlarge the snake by one

#endif
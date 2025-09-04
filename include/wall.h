// This header and C file are responsible on creating the wall object and checking collision between the snake and the wall
#pragma once

#ifndef WALL_H
#define WALL_H

#include "game_m~1.h" // For the position structure

typedef struct wall
{
	POSITION position;
	char appearence;
} WALL;

void spawn_wall(); // Creates a wall near the food item, pointing tawards the snake
void draw_wall(); // This function inserts the wall object into the display draft
int is_snake_on_wall(); // Checks collision between the snake and the wall object

extern WALL wall;

#endif
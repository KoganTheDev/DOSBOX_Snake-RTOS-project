#pragma once

#ifndef WALL_H
#define WALL_H

#define WALL_LENGTH 6

#include "game_m~1.h" // For the POSITION structure

typedef struct wall
{
    POSITION position;
    char appearance;  // fixed typo
} WALL;

void spawn_wall();       // Creates a wall near the food item, pointing towards the snake
void draw_wall();        // Inserts the wall object into the display draft
int is_snake_on_wall();  // Checks collision between the snake and the wall object

extern WALL wall[WALL_LENGTH];

#endif

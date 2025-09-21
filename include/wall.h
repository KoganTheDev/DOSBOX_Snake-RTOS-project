#pragma once

#ifndef WALL_H
#define WALL_H

#define MAX_WALL_LENGTH 16  // Maximum wall pieces (2 walls of 8 pieces each for level 4+)

#include "game_m~1.h" // For the POSITION structure

typedef struct wall
{
    POSITION position;
    char appearance;  // fixed typo
} WALL;

void spawn_wall();       // Creates wall(s) near the food item based on current level
void draw_wall();        // Inserts the wall object(s) into the display draft
int is_snake_on_wall();  // Checks collision between the snake and the wall object(s)

extern WALL wall[MAX_WALL_LENGTH];
extern int current_wall_count;  // Track how many wall pieces are currently active

// Helper functions for level-based walls
int get_wall_height_for_level();
int should_spawn_dual_walls();
void spawn_single_wall(int start_x, int start_y, int wall_height, int wall_start_index, int direction);

#endif

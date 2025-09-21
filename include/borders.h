// This header and C file are responsible for the hardcoded borders and collision against borders
#pragma once

#ifndef BORDERS_H
#define BORDERS_H

#include "game_m~1.h" // For the position structure

// The horizontal borders (north and south) need to span the full width (DISPLAY_COLS)
extern POSITION border_north[DISPLAY_COLS - 1];
extern POSITION border_south[DISPLAY_COLS];

// The vertical borders (east and west) need to span the full height (DISPLAY_ROWS)
extern POSITION border_east[DISPLAY_ROWS];
extern POSITION border_west[DISPLAY_ROWS];

void initialize_borders(); // Fills the borders with characters
void draw_borders(); // Insert the borders into the display array
int is_snake_on_border(); // If collision detected, returns 1, else 0

#endif

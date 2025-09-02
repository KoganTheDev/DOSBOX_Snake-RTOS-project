#pragma once

//! NOTE: To include this header use "#include <gane_m~1.h>", because fo DOSBOX`s limitations
//! NOTE: Used header gaurds because the compiler dicides to reinclude this folder which causes multiple definitions
#ifndef GAME_M~1_H
#define GAME_M~1_h

#define ARR_SIZE 1000
#define DISPLAY_ROWS 25
#define DISPLAY_COLS 40

#include <stdio.h>

typedef struct position
{
    int x; // X coordinate
    int y; // Y coordinate
} POSITION;


// A 2D array to serve as the drawing canvas for the game.
// It represents a 25-row, 40-column console screen.
extern char display_draft[DISPLAY_ROWS][DISPLAY_COLS];

extern int initial_run; // Flag to indicate if it's the first run of the game
extern int game_speed; // Game speed (delay in milliseconds)
extern int game_over; // Flag to indicate if the game is over

extern char display[DISPLAY_ROWS * DISPLAY_COLS + DISPLAY_ROWS + 1]; // Main display buffer (25 rows x 40 columns + null terminator) 





void clear_display_draft(void);


#endif


// This header and C file are responsible to most of the game logic
#pragma once

#ifndef GAME_M~1_H
#define GAME_M~1_h

#define ARR_SIZE 1000
#define DISPLAY_ROWS 25
#define DISPLAY_COLS 80

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
extern int level; // Represents the current level of the game
extern int is_level_up_food_and_wall_flag; // Flag that`s used to indicate a level up for the food and wall objects
extern int is_level_up_timer_flag; // Flag that`s used to indicate a level up for the game`s timer and allow a game speed limit
extern int prev_food_counter;

extern char display[DISPLAY_ROWS * DISPLAY_COLS + DISPLAY_ROWS + 1]; // Main display buffer (25 rows x 40 columns + null terminator) 

void clear_display_draft(); // Helper function - Clears the display draft by filling it with spaces.
void update_display_buffer(); // Helper function - Copies the display draft to the main display buffer.
void draw_game_elements(); // Draws the snake, food and the score on the display draft
void on_level_up(); // Turns on flags that cause changes in the way the game works
int is_level_up_food_and_wall(); // Turns on a flag that casues changes in the food and wall objects
int is_level_up_timer(); // Turns on a flag that causes changes in the game`s time limit and game speed



// --- MAIN LOOP FUNCTIONS ---

void displayer(); // This function is responsible for rendering the game state to the console.

// This function acts as a consumer for the keyboard input buffer.
// It moves the characters from `entered_ascii_codes` to the `ch_arr` queue,
// which is then processed by the `updater`.
void receiver();

// This is the main game logic function. It performs the following tasks:
// 1. Initializes the game state on the first run.
// 2. Processes user input from the `ch_arr` queue to control the game.
// TODO: after implementing full logic, add more documentation
void updater();

#endif


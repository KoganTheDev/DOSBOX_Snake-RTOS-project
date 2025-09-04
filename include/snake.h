#pragma once

#ifndef SNAKE_H
#define SNAKE_H

#include "game_m~1.h"

#define SNAKE_MAX_LENGTH 100

typedef struct snake
{
	POSITION body[SNAKE_MAX_LENGTH];
	int length; // Current length of the snake
	int direction; // Current direction of the snake - uses keyboard_scan_codes enum
} SNAKE;

extern SNAKE snake;

// A circular buffer for holding the processed keyboard commands.
extern char ch_arr[ARR_SIZE];
extern int front; // Index of the front of the queue
extern int rear; // Index of the rear of the queue


void initialize_snake(); // Initializes the snake in the center of the display.
void move_snake(); // Moves the snake in the current direction.
void update_snake_direction(); // Update the direction from the input character if it is a valid direction change.
void draw_snake(); // Draws the snake on the display draft.
int snake_self_collision(); // Checks if the snake`s head collided with a body segment 

#endif

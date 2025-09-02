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


#endif

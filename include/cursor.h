// This header and C file are responsible on enlarging the cursor and keeping it on the snake`s head
#pragma once

#ifndef CURSOR_H
#define CURSOR_H

#include "snake.h" // Used to follow the snake`s head location with the cursor
#include "game_m~1.h"

// Define constants for the CRT controller ports
#define CRT_INDEX_PORT 0x3D4
#define CRT_DATA_PORT  0x3D5

// Define constants for the CRT registers
#define CURSOR_START_REG      0x0A
#define CURSOR_END_REG        0x0B
#define CURSOR_HIGH_POS_REG   0x0E
#define CURSOR_LOW_POS_REG    0x0F

void set_full_block_cursor(); // This function enlarges the cursor 
void move_cursor_to_snake_head(); // This function makes sure that the cursor follows the snake`s head

#endif 

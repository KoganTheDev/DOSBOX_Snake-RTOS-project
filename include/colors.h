#pragma once

#ifndef COLORS_H
#define COLORS_H

#include "game_m~1.h"

#define BLACK       0x00
#define BLUE        0x01 | 0x8
#define GREEN       0x02 | 0x8
#define RED         0x04 | 0x8
#define MAGENTA     0x05 | 0x8
#define LIGHTGRAY   0x07
#define YELLOW      0x0E
#define ORANGE      0x06
#define WHITE       0x0F
#define GREY_BACKGROUND 0x70

typedef struct
{
    char snake_head;
    char snake_body_segment;
    char wall;
    char borders;
    char food;
    char screen;
} color_set;

extern const color_set game_color_sets[10];
extern char display_colors[DISPLAY_ROWS][DISPLAY_COLS];

// This function updates the color draft with different colors for each element
void color_display_draft();

#endif

#include "borders.h"
#include "snake.h" // used to check collision against the snake object
#include "music.h"

POSITION border_north[DISPLAY_COLS - 1];
POSITION border_south[DISPLAY_COLS];
POSITION border_east[DISPLAY_ROWS];
POSITION border_west[DISPLAY_ROWS];

// Globals relevant only to this file
int first_row = 1;
int last_row = DISPLAY_ROWS - 1;
int last_col = DISPLAY_COLS - 1;

void initialize_borders()
{
    int i;

    for (i = 0; i < DISPLAY_COLS; i++)
    {
        // Top border is now on y-coordinate 1 (the second row)
        border_north[i].x = i;
        border_north[i].y = first_row;

        // Bottom border is on the last row
        border_south[i].x = i;
        border_south[i].y = last_row;
    }

    // Left and right borders now start from y=1 to avoid the score row
    for (i = 1; i < last_row; i++)
    {
        // Left border
        border_west[i].x = 0;
        border_west[i].y = i;

        // Right border
        border_east[i].x = last_col;
        border_east[i].y = i;
    }
}

void draw_borders()
{
    int i;


    // Draw horizontal borders
    for (i = 0; i < DISPLAY_COLS; i++)
    {
        // Top border
        display_draft[border_north[i].y][border_north[i].x] = '-';

        // Bottom border
        display_draft[border_south[i].y][border_south[i].x] = '-';
    }

    // Draw vertical borders
    for (i = first_row; i < last_row; i++)
    {
        // Left border
        display_draft[border_west[i].y][border_west[i].x] = '|';

        // Right border
        display_draft[border_east[i].y][border_east[i].x] = '|';
    }

    // Draw the corners of the border on the second row
    display_draft[first_row][0] = '+';
    display_draft[first_row][last_col] = '+';
    display_draft[last_row][0] = '+';
    display_draft[last_row][last_col] = '+';
}

int is_snake_on_border()
{
    POSITION snake_head = snake.body[0];

    // Get border coordinates from the border arrays
    int north_y = 1;
    int south_y = DISPLAY_ROWS - 1;
    int west_x = 0;
    int east_x = DISPLAY_COLS - 1;

    // Check for collision with any of the four borders
    if (snake_head.x == west_x ||
        snake_head.x == east_x ||
        snake_head.y == north_y ||
        snake_head.y == south_y)
    {
        play_snake_dies_sound();
        return 1; // Collision detected
    }

    return 0; // No collision
}
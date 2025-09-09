#include "colors.h"

char display_colors[DISPLAY_ROWS][DISPLAY_COLS];

const color_set game_color_sets[10] =
{
    // 0: Classic High Contrast - White on Black
    { (BLACK << 4) | WHITE, (BLACK << 4) | WHITE, (BLACK << 4) | WHITE, (BLACK << 4) | WHITE, (BLACK << 4) | WHITE, (BLACK << 4) | BLACK },
    // 1: Blue on Black
    { (BLACK << 4) | BLUE, (BLACK << 4) | BLUE, (BLACK << 4) | BLUE, (BLACK << 4) | BLUE, (BLACK << 4) | BLUE, (BLACK << 4) | BLACK },
    // 2: Green on Black
    { (BLACK << 4) | GREEN, (BLACK << 4) | GREEN, (BLACK << 4) | GREEN, (BLACK << 4) | GREEN, (BLACK << 4) | GREEN, (BLACK << 4) | BLACK },
    // 3: Yellow on Black
    { (BLACK << 4) | YELLOW, (BLACK << 4) | YELLOW, (BLACK << 4) | YELLOW, (BLACK << 4) | YELLOW, (BLACK << 4) | YELLOW, (BLACK << 4) | BLACK },
    // 4: Red on Black
    { (BLACK << 4) | RED, (BLACK << 4) | RED, (BLACK << 4) | RED, (BLACK << 4) | RED, (BLACK << 4) | RED, (BLACK << 4) | BLACK },
    // 5: Black on Gray
    { GREY_BACKGROUND | BLACK, GREY_BACKGROUND | BLACK, GREY_BACKGROUND | BLACK, GREY_BACKGROUND | BLACK, GREY_BACKGROUND | BLACK, GREY_BACKGROUND },
    // 6: Blue on Gray
    { GREY_BACKGROUND | BLUE, GREY_BACKGROUND | BLUE, GREY_BACKGROUND | BLUE, GREY_BACKGROUND | BLUE, GREY_BACKGROUND | BLUE, GREY_BACKGROUND },
    // 7: Green on Gray
    { GREY_BACKGROUND | GREEN, GREY_BACKGROUND | GREEN, GREY_BACKGROUND | GREEN, GREY_BACKGROUND | GREEN, GREY_BACKGROUND | GREEN, GREY_BACKGROUND },
    // 8: Red on Gray
    { GREY_BACKGROUND | RED, GREY_BACKGROUND | RED, GREY_BACKGROUND | RED, GREY_BACKGROUND | RED, GREY_BACKGROUND | RED, GREY_BACKGROUND },
    // 9: Magenta on Gray
    { GREY_BACKGROUND | MAGENTA, GREY_BACKGROUND | MAGENTA, GREY_BACKGROUND | MAGENTA, GREY_BACKGROUND | MAGENTA, GREY_BACKGROUND | MAGENTA, GREY_BACKGROUND }
};

// TODO: delete later when difficulty is implemented
int k = 0; // Simulate cyclic color change

void color_display_draft()
{
    char char_game_element; // Holds the in game character for the element
    int color_index = k % 10; // For 15 different color sets
    int i;
    int j;
    int second_row = 1;

    for (i = 1; i < DISPLAY_ROWS; i++)
    {
        for (j = 0; j < DISPLAY_COLS; j++)
        {
            char_game_element = display_draft[i][j];

            switch (char_game_element)
            {
            case ('@'): // Snake head
                display_colors[i][j] = game_color_sets[color_index].snake_head;
                break;
            case ('|'): // Snake vertical body segment
            case ('='): // Snake horizontal body segment
                if (j == 0 || j == DISPLAY_COLS - 1) // Pipes (`|`) are also used for the eastern and southern borders
                {
                    display_colors[i][j] = game_color_sets[color_index].borders;
                }
                else // It`s a snake body segment
                {
                    display_colors[i][j] = game_color_sets[color_index].snake_body_segment;
                }
                break;
            case ('>'): // Right wall
            case ('<'): // Left wall
                display_colors[i][j] = game_color_sets[color_index].wall;
                break;
            case ('*'): // Food
                display_colors[i][j] = game_color_sets[color_index].food;
                break;
            case (' '):
                display_colors[i][j] = game_color_sets[color_index].screen;
            default:
                // If it's a border character, set border color
                if (i == second_row || i == DISPLAY_ROWS - 1 || j == 0 || j == DISPLAY_COLS - 1) {
                    display_colors[i][j] = game_color_sets[color_index].borders;
                }
                break;
            }
        }
    }
    k++;
}

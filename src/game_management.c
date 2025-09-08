#include "game_m~1.h"
#include "snake.h"
#include "food.h"
#include "score.h"
#include "keyboa~1.h"
#include "borders.h"
#include "cursor.h"
#include "clock.h"
#include "wall.h"


char display[DISPLAY_ROWS * DISPLAY_COLS + DISPLAY_ROWS + 1];
char display_draft[DISPLAY_ROWS][DISPLAY_COLS];
int initial_run = 1; // Flag to indicate if it's the first run of the game
int game_speed = 100; // Game speed (delay in milliseconds)
int game_over = 0; // Flag to indicate if the game is over


void clear_display_draft()
{
    int i;
    int j;

    for (i = 0; i < DISPLAY_ROWS; i++)
    {
        for (j = 0; j < DISPLAY_COLS; j++)
        {
            display_draft[i][j] = ' ';
        }
    }
}

void update_display_buffer()
{
    int index = 0;
    int i;
    int j;

    for (i = 0; i < DISPLAY_ROWS; i++)
    {
        for (j = 0; j < DISPLAY_COLS; j++)
        {
            display[index++] = display_draft[i][j];
        }
    }
    display[index] = '\0'; // Null-terminate the string
}

void draw_game_elements()
{
    char buffer[DISPLAY_COLS]; // A temp buffer for the entire top row
    int remaining_time;
    int x;
    int y;
    int i;
    int level = 1; // TODO: Change when dificulty is implemented
    
    draw_borders();
    draw_snake();

    // TODO: Implement different sizes for the food for difficulty
    // Draw the food
    draw_food();

    // Draw the Wall object
    draw_wall();

    // Calculate and display the remainng time
    remaining_time = get_remaining_time();

    // Draw the score at the top-left corner
    sprintf(buffer, "Level: %d | Score %d | Time %d", level, score, remaining_time);
    for (i = 0; buffer[i] != '\0'; i++)
    {
        display_draft[0][i] = buffer[i];
    }


    // TODO: Add timer and time display
}

//! CRITICAL: handles smooth printing and colors, if possible refactor
void displayer()
{
    char far* video_memory = (char far*)0xB8000000;
    int i, j;
    int buffer_index = 0;

    // Iterate through the display buffer and copy characters and attributes
    for (i = 0; i < DISPLAY_ROWS; i++) {
        for (j = 0; j < DISPLAY_COLS; j++) {
            // Write the character
            *(video_memory + (i * 80 + j) * 2) = display_draft[i][j];
            // Write the color attribute (e.g., light gray on black)
            *(video_memory + (i * 80 + j) * 2 + 1) = 0x07;
        }
    }
}

void receiver()
{
    int i = 0;
    char temp;

    // Loop through all characters in the input buffer.
    while (i <= tail)
    {
        temp = entered_ascii_codes[i];
        if (rear < ARR_SIZE - 1)
        {
            rear++;
            ch_arr[rear] = temp;
        }
        i++;

        if (front == -1)
        {
            front = 0;
        }
    }

    // reset the input buffer pointer.
    tail = -1;
}


void updater()
{
    // Initial setup for the first run of the game.
    if (initial_run == 1)
    {
        initialize_borders();
        initialize_snake();
        spawn_food(); // Spawn the first food item, uses srand seed
        spawn_wall(); // Spawn the first wall near the food
        set_full_block_cursor(); // Enlarge the cursor`s size

        // Initialize the clock and start the countdown
        set_clock_handler();
        start_countdown();

        initial_run = 0;
    }

    update_snake_direction();
    move_snake();

    // TODO: put collision detection here. 
    // TODO: detection against food, walls and out-of-bounds

    if (is_snake_on_border() ||
        snake_self_collision() ||
        is_snake_on_wall() ||
        get_remaining_time() == -1) //! Used -1 so we won`t get game over due to timeout, need to be fixed
    {
        delay(300); // Allows the player to understand the cause of the problem
        game_over = 1;
        restore_keyboard_handler();
        restore_clock_handler(); // Remove the clock ISR
    }

    if (!game_over)
    {
        clear_display_draft(); // Clear the display draft before drawing
        draw_game_elements(); // Draw the snake, food, and score
        update_display_buffer(); // Copy the draft to the main display buffer
        displayer(); // Display the content of the game
        move_cursor_to_snake_head();
    }
    else
    {
        print_game_over_screen();
    }
}

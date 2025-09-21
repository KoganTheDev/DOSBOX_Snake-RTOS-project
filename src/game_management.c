#include "game_m~1.h"
#include "snake.h"
#include "food.h"
#include "score.h"
#include "keyboa~1.h"
#include "borders.h"
#include "cursor.h"
#include "clock.h"
#include "wall.h"
#include "colors.h"

char display[DISPLAY_ROWS * DISPLAY_COLS + DISPLAY_ROWS + 1];
char display_draft[DISPLAY_ROWS][DISPLAY_COLS];
int initial_run = 1; // Flag to indicate if it's the first run of the game
int game_speed = 100; // Game speed (delay in milliseconds)
int game_over = 0; // Flag to indicate if the game is over
int level = 1;

int is_level_up_food_and_wall_flag = 0; // Flag that`s used to indicate a level up for the food and wall objects
int is_level_up_timer_flag = 0; // Flag that`s used to indicate a level up for the game`s timer and allow a game speed limit
int prev_food_counter = 0;


int is_level_up_food_and_wall()
{
    if (is_level_up_food_and_wall_flag)
    {
        is_level_up_food_and_wall_flag = 0;
        return 1; // Level up
    }

    return 0; // No level up
}

int is_level_up_timer()
{
    if (is_level_up_timer_flag)
    {
        is_level_up_timer_flag = 0;
        return 1; // Level up
    }

    return 0; // No level up
}


void on_level_up()
{
    level++;
    prev_food_counter = food_eaten;
    play_level_up_sound();
    is_level_up_food_and_wall_flag = 1;
    is_level_up_timer_flag = 1;
}

void clear_display_draft()
{
    int i;
    int j;

    for (i = 0; i < DISPLAY_ROWS; i++)
    {
        for (j = 0; j < DISPLAY_COLS; j++)
        {
            display_draft[i][j] = ' ';
            display_colors[i][j] = 0x07; // Default color - grey
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
    
    draw_borders();
    draw_snake();
    draw_food();     // Draw the food
    draw_wall();     // Draw the Wall object

    // Calculate and display the remainng time
    remaining_time = get_remaining_time();

    // Draw the score at the top-left corner
    sprintf(buffer, "Level: %d | Food Eaten %d | Score %d | Time %d", level, food_eaten, score, remaining_time);
    for (i = 0; buffer[i] != '\0'; i++)
    {
        display_draft[0][i] = buffer[i];
    }
}

void displayer()
{
    char far* video_memory = (char far*)0xB8000000;
    int i, j;

    // Iterate through the display buffer and copy characters and attributes
    for (i = 0; i < DISPLAY_ROWS; i++) {
        for (j = 0; j < DISPLAY_COLS; j++) {
            // Write the character
            *(video_memory + (i * 80 + j) * 2) = display_draft[i][j];

            // Write the color attribute from the array display colors
            *(video_memory + (i * 80 + j) * 2 + 1) = display_colors[i][j];
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

    // Check for a level up
    // Level up is caused when the snake eats another 2 fruit objects
    if (prev_food_counter + 2 == food_eaten)
    {
        on_level_up();
        // Immediately regenerate walls for the new level
        regenerate_walls_for_level_change();
    }

    if (is_snake_on_border() ||
        snake_self_collision() ||
        is_snake_on_wall() ||
        get_remaining_time() == 0)
    {
        delay(300); // Allows the player to understand the cause of the problem
        game_over = 1;
        restore_keyboard_handler();
        restore_clock_handler(); // Remove the clock ISR
    }

    if (!game_over)
    {
        clear_display_draft(); // Clear the display draft before drawing
        if (is_level_up_timer()) {
            if ((countdown_seconds - 30) < 15) {
                countdown_seconds = 15;
            }
            else {
                countdown_seconds -= 25;
            }
            if (game_speed > 30) {
                game_speed -= 10;
            }
        }
        draw_game_elements(); // Draw the snake, food, and score
        color_display_draft(); // Add colors by inserting them to "the "display_colors" array, which then will be used to color the game screen
        update_display_buffer(); // Copy the draft to the main display buffer
        displayer(); // Display the content of the game
        move_cursor_to_snake_head();
    }
    else
    {
        print_game_over_screen();
    }
}

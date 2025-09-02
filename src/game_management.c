#include "game_m~1.h"
#include "snake.h"
#include "food.h"
#include "score.h"
#include "keyboa~1.h"

char display[DISPLAY_ROWS * DISPLAY_COLS + DISPLAY_ROWS + 1];
char display_draft[DISPLAY_ROWS][DISPLAY_COLS];
int initial_run = 1; // Flag to indicate if it's the first run of the game
int game_speed = 1000; // Game speed (delay in milliseconds)
int game_over = 0; // Flag to indicate if the game is over




// Helper function - Clears the display draft by filling it with spaces.
void clear_display_draft(void)
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

// Helper function - Copies the display draft to the main display buffer.
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
        display[index++] = '\n';
    }
    display[index] = '\0'; // Null-terminate the string
}

// Draws the snake, food and the score on the display draft
void draw_game_elements()
{
    char score_text[20];
    int x;
    int y;
    int i;

    // Draw the snake
    for (i = 0; i < snake.length; i++)
    {
        x = snake.body[i].x;
        y = snake.body[i].y;

        if (0 <= x && x < DISPLAY_COLS && 0 <= y && y < DISPLAY_ROWS)
        {
            display_draft[y][x] = (i == 0) ? '@' : '='; // Head is '@', body is '='
        }
    }

    // TODO: Implement different sizes for the food for difficulty
    // Draw the food
    display_draft[food.y][food.x] = '*'; // Food is '*'

    // Draw the score at the top-left corner
    sprintf(score_text, "Score: %d", score);
    for (i = 0; score_text[i] != '\0'; i++)
    {
        display_draft[0][i] = score_text[i];
    }

    // TODO: Add timer and time display
}

// --- MAIN LOOP FUNCTIONS ---

// This function is responsible for rendering the game state to the console.
void displayer(void)
{
    clrscr();
    printf("%s", display); // Print the entire screen buffer
}

// This function acts as a consumer for the keyboard input buffer.
// It moves the characters from `entered_ascii_codes` to the `ch_arr` queue,
// which is then processed by the `updater`.
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

// This is the main game logic function. It performs the following tasks:
// 1. Initializes the game state on the first run.
// 2. Processes user input from the `ch_arr` queue to control the game.
// TODO: after implementing full logic, add more documentation
void updater()
{
    // Initial setup for the first run of the game.
    if (initial_run == 1)
    {
        initialize_snake();
        srand(time(NULL)); // Seed the random number generator
        spawn_food(); // Spawn the first food item, uses srand seed
        initial_run = 0;
    }

    update_snake_direction();
    move_snake();

    // TODO: put collision detection here. 
    // TODO: detection against food, walls and out-of-bounds

    if (!game_over)
    {
        clear_display_draft(); // Clear the display draft before drawing
        draw_game_elements(); // Draw the snake, food, and score
        update_display_buffer(); // Copy the draft to the main display buffer
    }
    else
    {
        print_game_over_screen();
    }
}


// This function is used to gracefully terminate the program
// it restores the original keyboard interrupt handler and then exits.
void halt()
{


    restore_keyboard_handler();

    print_thank_you_message_screen();
    clrscr(); // Clear the console (Turbo C function)

    exit(0);
}
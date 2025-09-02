#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <conio.h>

#include "music.h"

#define ARR_SIZE 1000
#define DISPLAY_ROWS 25
#define DISPLAY_COLS 40
#define SNAKE_MAX_LENGTH 100
#define KEYBOARD_HARDWARE_INTERRUPT 0x09

typedef struct position
{
    int x; // X coordinate
    int y; // Y coordinate
} POSITION;

typedef struct snake
{
    POSITION body[SNAKE_MAX_LENGTH];
	int length; // Current length of the snake
	int direction; // Current direction of the snake - uses keyboard_scan_codes enum
} SNAKE;

// Enumeration for different keyboard scan codes
enum keyboard_scan_codes
{
	ESCAPE_KEY = 1,
	UP_ARROW = 72,
	LEFT_ARROW = 75,
	RIGHT_ARROW = 77,
	DOWN_ARROW = 80
};

// --- Global variables ---
void interrupt (*old_keyboard_handler)(void); // Pointer to the old keyboard interrupt handler

SNAKE snake;
POSITION food; // Position of the food
int initial_run = 1; // Flag to indicate if it's the first run of the game
int score = 0; // Player's score
int game_speed = 1000; // Game speed (delay in milliseconds)
int game_over = 0; // Flag to indicate if the game is over

// A 2D array to serve as the drawing canvas for the game.
// It represents a 25-row, 40-column console screen.
char display_draft[DISPLAY_ROWS][DISPLAY_COLS];

char entered_ascii_codes[ARR_SIZE]; // Buffer for raw ASCII codes from keyboard
int tail = -1; // Index of the last entered ASCII code

char display[DISPLAY_ROWS * DISPLAY_COLS + DISPLAY_ROWS + 1]; // Main display buffer (25 rows x 40 columns + null terminator) 

// A circular buffer for holding the processed keyboard commands.
char ch_arr[ARR_SIZE];
int front = -1; // Index of the front of the queue
int rear = -1; // Index of the rear of the queue




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


void print_welcome_message()
{
    clrscr(); // Clear the console (Turbo C function)

    printf("\n\n\n");
    printf("                *****************************************************\n");
    printf("                *                                                   *\n");
    printf("                *        WELCOME TO THE DOSBOX SNAKE GAME!          *\n");
    printf("                *                                                   *\n");
    printf("                *****************************************************\n");
    printf("\n\n");
    printf("                Use the arrow keys to control the snake.\n");
    printf("                Press ESC to exit the game at any time.\n");
    printf("                Press any key to start...\n");

    play_welcome_message_melody(); // Play a simple melody until a key is pressed
}

// This function is used to gracefully terminate the program
// it restores the original keyboard interrupt handler and then exits.
void halt()
{


	// Restore the original keyboard interrupt handler
	setvect(KEYBOARD_HARDWARE_INTERRUPT, old_keyboard_handler); 
	asm { CLI } // Disable incomming interrupts
    
    clrscr(); // Clear the console (Turbo C function)

    // Centered ASCII art message (fits 40 columns)
    printf("\n\n\n");
    printf("                *****************************************************\n");
    printf("                *                                                   *\n");
    printf("                *        THANK YOU FOR PLAYING DOSBOX SNAKE!        *\n");
    printf("                *                                                   *\n");
    printf("                *****************************************************\n");
    printf("\n\n");
    printf("                Press any key to exit...\n");

	play_thank_you_message_melody(); // Play a simple melody until a key is pressed
    exit(0); 
}

// Spawns food at a random position not occupied by the snake.
void spawn_food()
{
    int i;
    int on_snake;

    do
    {
        on_snake = 0;
        food.x = rand() % DISPLAY_COLS; // Random x position
        food.y = rand() % DISPLAY_ROWS; // Random y position
        
        // Ensure the food does not spawn on the snake
        for (i = 0; i < snake.length; i++)
        {
            if (snake.body[i].x == food.x && snake.body[i].y == food.y)
            {
                on_snake = 1;
                break;
            }
		}
	} while (on_snake);
}


// Initializes the snake in the center of the display.
void initialize_snake()
{
    int start_x;
    int start_y;
    int i;

    // TODO: for difficulty, create a variable for initial length, when leveling up,
   // TODO: increase the length by 1 or 2.
    snake.length = 5; // Initial length of the snake
    snake.direction = RIGHT_ARROW; // Initial direction of the snake

    // Position the snake in the center of the display
    start_x = DISPLAY_COLS / 2;
    start_y = DISPLAY_ROWS / 2;


    for (i = 0; i < snake.length; i++)
    {
		// TODO: Adjust here if the snake is out-of-bounds at start.
        snake.body[i].x = start_x - i; // Horizontal placement
		snake.body[i].y = start_y; // Vertical placement
    }
}


// Moves the snake in the current direction.
void move_snake()
{
    int i;
    POSITION* head = &snake.body[0];
	POSITION prev_head = *head; // Store previous head position

	// Move body segments
    for (i = snake.length - 1; i > 0; i--)
    {
        snake.body[i] = snake.body[i - 1];
	}

	// Update head position based on direction
	head = &snake.body[0];
    
    // Update the head position based on direction
    switch (snake.direction)
    {
        case UP_ARROW:
            head->y--; // Go up
            break;
        case DOWN_ARROW:
            head->y++; // Go down
            break;
        case LEFT_ARROW:
			head->x--; // Go left
            break;
        case RIGHT_ARROW:
			head->x++; // Go right
			break;
    }

    // Check if the snale has eaten the food
    if (head->x == food.x && head->y == food.y)
    {
		// Increase the length of the snake
        if (snake.length < SNAKE_MAX_LENGTH)
        {
            for (i = snake.length; i > 0; i--)
            {
                snake.body[i] = snake.body[i - 1];
			}
            snake.body[0] = *head;
            snake.length++;
        }

        // Increase Score
        // TODO: ADD score multiplayer for fun!
		score += 10; 

        // TODO: Implement level up mechanism by making the game harder

        // TODO: Maybe do somehting cool like displaying a message or sound effect
		spawn_food(); // Spawn new target for the snake
    }
}

// TODO: Implement collision detection with walls


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

// Draws the snake on the display draft.
void draw_snake()
{
    int i;

    for (i = 0; i < snake.length; i++)
    {
        int x = snake.body[i].x;
        int y = snake.body[i].y;

        if (0 <= x && x < DISPLAY_COLS && 0 <= y && y < DISPLAY_ROWS)
        {
            display_draft[y][x] = (i == 0) ? '@' : '='; // Head is '@', body is '='
        }
    }
}

// Update the direction from the input character if it is a valid direction change.
void update_snake_direction()
{
    if (front != -1 && rear >= front)
    {
		int new_direction = ch_arr[front++];
        
        // Ensure the new direction is not directly opposite to the current direction
        if ((new_direction == UP_ARROW && snake.direction != DOWN_ARROW) ||
            (new_direction == DOWN_ARROW && snake.direction != UP_ARROW) ||
            (new_direction == LEFT_ARROW && snake.direction != RIGHT_ARROW) ||
            (new_direction == RIGHT_ARROW && snake.direction != LEFT_ARROW))
        {
            snake.direction = new_direction;
		}
        if (front > rear) // Reset queue if empty
        {
            front = -1;
			rear = -1;
        }
    }
}

// This is a custom keyboard interrupt handler.
// It`s called every time a key is pressed. It reads the scan code and ASCII code of the key pressed without consuming it.
// It then stores the ASCII code in the `entered_ascii_codes` buffer if a valid key was pressed and the buffer isn't full.
// TODO: Refactor this function to improve readability.
// TODO: Change logic to fit the logic for the snake game.
// TODO: remove assembly code if possible.
void interrupt new_keyboard_handler(void)
{
    int scan = 0;

    // Chain to the old interrupt handler to allow the system to process the key.
    (*old_keyboard_handler)();

    // Inline assembly to check for keyboard input without removing it.
    // INT 16h with AH=1 checks for a keypress. JZ (Jump if Zero) skips if no key is pressed.
    // If a key is pressed, INT 16h with AH=0 reads it and stores the scan code (AH) and ASCII (AL).
    asm{
       MOV AH,1
       INT 16h
       JZ Skip1
       MOV AH,0
       INT 16h
       MOV BYTE PTR scan,AH
       // MOV BYTE PTR ascii,AL 
    } //asm
Skip1:;


     // TODO: implement CTRL-C handling using the line down here if possible, if breaks something - remove.
    // if ((scan == 46)&& (ascii == 3)) // Ctrl-C? Not used

    // Make sure to store only relavant keys for the game.
    if (((scan == UP_ARROW) ||
        (scan == DOWN_ARROW) ||
        (scan == LEFT_ARROW) ||
        (scan == RIGHT_ARROW) ||
        (scan == ESCAPE_KEY)) 
        && (tail < ARR_SIZE - 1))
    {
        entered_ascii_codes[++tail] = scan; // Store the scan code directly
    }
    if (scan == ESCAPE_KEY)
    {
		halt(); // Terminate the program gracefully
    }
}

// --- Main game loop functions ---

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
        // TODO: GAME OVER SCREEN
        // Game Over screen logic
        /*
        clrscr();
        printf("\n\n\n");
        printf("                  *****************************************************\n");
        printf("                  * *\n");
        printf("                  * GAME OVER!                    *\n");
        printf("                  * *\n");
        printf("                  *****************************************************\n");
        printf("\n\n");
        printf("                      Your final score was: %d\n", score);
        printf("\n\n");
        printf("                  Press any key to exit...\n");
        getch();
        halt();
        */  // TODO: check if halt() causes problems because it will print the goodbye screen for no reason,
		// TODO: maybe add sleep before calling halt() or refactor halt() to not print the goodbye screen if game over.
    }
}

void main()
{
    print_welcome_message();
    clear_display_draft(); // Clear the display draft at the start

    // Save the address of the old keyboard interrupt handler.
    old_keyboard_handler = getvect(KEYBOARD_HARDWARE_INTERRUPT);

	// Set the new keyboard interrupt handler.
	setvect(KEYBOARD_HARDWARE_INTERRUPT, new_keyboard_handler);

    while (1)
    {
        receiver(); // Get and process user input from the interrupt handler.
        updater(); // Update game logic and render the screen draft.
        displayer(); // Print the screen to the console.


        // Sleep for a short duration to control the game speed.
        delay(game_speed); // Equivalent to sleep(1) in seconds.
    }
}
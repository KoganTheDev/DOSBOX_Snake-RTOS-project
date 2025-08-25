#include <stdio.h>
#include <dos.h>

#define ARR_SIZE 1000

#define KEYBOARD_HARDWARE_INTERRUPT 0x09

typedef struct position
{
    int x; // X coordinate
    int y; // Y coordinate
} POSITION;

// Enumeration for different keyboard scan codes
enum keyboard_scan_codes
{
	ESCAPE_KEY = 1,
	UP_ARROW = 72,
	LEFT_ARROW = 75,
	RIGHT_ARROW = 77,
	DOWN_ARROW = 80
};

void interrupt(*old_keyboard_handler)(void); // Pointer to the old keyboard interrupt handler

char entered_ascii_codes[ARRSIZE]; // Buffer for raw ASCII codes from keyboard
int tail = -1; // Index of the last entered ASCII code

// Main display buffer (25 rows x 80 columns + null terminator) 
char display[2001]

// A circular buffer for holding the processed keyboard commands.
ch_arr[ARR_SIZE];
int front = -1; // Index of the front of the queue
int rear = -1; // Index of the rear of the queue

// This function is used to gracefully terminate the program
// it restores the original keyboard interrupt handler and then exits.
void halt()
{
	// Restore the original keyboard interrupt handler
	setvect(KEYBOARD_HARDWARE_INTERRUPT, old_keyboard_handler); 
	asm { CLI } // Disable incomming interrupts
	exit(0); 
}

// This is a custom keyboard interrupt handler.
// It`s called every time a key is pressed. It reads the scan code and ASCII code of the key pressed without consuming it.
// It then stores the ASCII code in the `entered_ascii_codes` buffer if a valid key was pressed and the buffer isn't full.
// TODO: Refactor this function to improve readability.
// TODO: Change logic to fit the logic for the snake game.
// TODO: remove assembly code if possible.
void interrupt new_keyboard_handler(void)
{
    char result = 0;
    int scan = 0;
    int ascii = 0;

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
       MOV BYTE PTR ascii,AL
    } //asm

    ascii = 0;
    // The following if-else block maps the scan codes of the arrow keys to specific
    // ASCII characters used for game control.
    if (scan == LEFT_ARROW) // Scan code for Left Arrow
        ascii = 'a';
    else
        if (scan == UP_ARROW) // Scan code for Up Arrow
            ascii = 'w';
        else
            if (scan == RIGHT_ARROW) // Scan code for Right Arrow
                ascii = 'd';
            else
                if (scan == DOWN_ARROW) // Scan code for Down Arrow
					ascii = 's';

    // TODO: implement CTRL-C handling using the line down here if possible, if breaks something - remove.
    // if ((scan == 46)&& (ascii == 3)) // Ctrl-C? Not used
    if (scan == ESCAPE_KEY) // Scan code for Escape
    {
        halt(); // Terminate the program gracefully
    }

    // Store the mapped ASCII code in the `entered_ascii_codes` buffer if a valid
    // key was pressed and the buffer isn't full.
    if ((ascii != 0) && (tail < ARRSIZE))
    {
        entered_ascii_codes[++tail] = ascii;
    }

Skip1:; // Label for the jump instruction in the assembly block.
}


// This function is responsible for rendering the game state to the console.
void displayer(void)
{
    printf(display); // Print the entire screen buffer
}

// This function acts as a consumer for the keyboard input buffer.
// It moves the characters from `entered_ascii_codes` to the `ch_arr` queue,
// which is then processed by the `updater`.
void receiver()
{
    char temp;
    int i;

    // Loop through all characters in the input buffer.
    while (i <= tail)
    {
        temp = entered_ascii_codes[i];
        rear++;
        i++;

        // Enqueue the character into the `ch_arr` queue.
        if (rear < ARRSIZE)
        {
            ch_arr[rear] = temp;
        }

        if (front == -1)
        {
            front = 0;
        }
	}

	// reset the input buffer pointer.
    tail = 0;
}

// A 2D array to serve as the drawing canvas for the game.
// It represents a 25-row, 80-column console screen.
char display_draft[25][80];

// TODO: add possion structures for the snake, the food and the walls.

//

// This is the main game logic function. It performs the following tasks:
// 1. Initializes the game state on the first run.
// 2. Processes user input from the `ch_arr` queue to control the game.
// TODO: after implementing full logic, add more documentation
void updater()
{
    int i, j;

    // Initial setup for the first run of the game.
    if (initial_run == 1)
    {
		// TODO: implement initial setup for the snake game.
		// ! use space_invaders.c as a reference.
    }
}

int main()
{
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
        sleep(1); // equals to delay(2700);
    }
}
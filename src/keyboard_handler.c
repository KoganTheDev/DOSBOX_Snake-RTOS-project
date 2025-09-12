#include "keyboa~1.h"
#include <dos.h>

void interrupt (*old_keyboard_handler)(void);

char entered_ascii_codes[ARR_SIZE]; // Buffer for raw ASCII codes from keyboard
int tail = 0; // Index of the last entered ASCII code
int head = 0;

void interrupt new_keyboard_handler(void)
{
    int scan_code = 0;

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
       MOV BYTE PTR scan_code,AH
        // MOV BYTE PTR ascii,AL 
    } //asm
Skip1:;

    // Make sure to store only relavant keys for the game.
    if (((scan_code == UP_ARROW) ||
        (scan_code == DOWN_ARROW) ||
        (scan_code == LEFT_ARROW) ||
        (scan_code == RIGHT_ARROW) ||
        (scan_code == ESCAPE_KEY))
        && (tail < ARR_SIZE - 1))
    {
        _disable(); // Disable interrupts to prevent a race condition with the main loop
        entered_ascii_codes[++tail] = scan_code; // Store the scan code directly
        _enable(); // Re-enable interrupts
    }
    if (scan_code == ESCAPE_KEY)
    {
        // Restore ISR handlers to prevent a crash
        restore_keyboard_handler();
        restore_clock_handler();

        print_thank_you_message_screen(); 
    }
}

void set_keyboard_handler(void)
{
    // Save the address of the old keyboard interrupt handler.
    old_keyboard_handler = getvect(KEYBOARD_HARDWARE_INTERRUPT);

    // Set the new keyboard interrupt handler.
    setvect(KEYBOARD_HARDWARE_INTERRUPT, new_keyboard_handler);
}

void restore_keyboard_handler()
{
	setvect(KEYBOARD_HARDWARE_INTERRUPT, old_keyboard_handler); 
	asm { CLI } // Disable incomming interrupts
}

int get_next_key_from_quque()
{
    int key;

    // Queue empty
    if (head == tail)
    {
        return 0;
    }
    else
    {
        _disable(); // CLI flag on - Clear interrupt flag, ignore incoming hardware interrupts
        key = entered_ascii_codes[head];
        head = (head + 1) % ARR_SIZE;
        _enable(); // STI flag on - Allow incoming hardware interrupts to be intercepted
        return key;
    }
}

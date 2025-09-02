#include "keyboa~1.h"

void interrupt (*old_keyboard_handler)(void);

char entered_ascii_codes[ARR_SIZE]; // Buffer for raw ASCII codes from keyboard
int tail = -1; // Index of the last entered ASCII code

//// This is a custom keyboard interrupt handler.
//// It`s called every time a key is pressed. It reads the scan code and ASCII code of the key pressed without consuming it.
//// It then stores the ASCII code in the `entered_ascii_codes` buffer if a valid key was pressed and the buffer isn't full.
//// TODO: Refactor this function to improve readability.
//// TODO: Change logic to fit the logic for the snake game.
//// TODO: remove assembly code if possible.
//void interrupt new_keyboard_handler(void)
//{
//    int scan = 0;
//
//    // Chain to the old interrupt handler to allow the system to process the key.
//    (*old_keyboard_handler)();
//
//    // Inline assembly to check for keyboard input without removing it.
//    // INT 16h with AH=1 checks for a keypress. JZ (Jump if Zero) skips if no key is pressed.
//    // If a key is pressed, INT 16h with AH=0 reads it and stores the scan code (AH) and ASCII (AL).
//    asm{
//       MOV AH,1
//       INT 16h
//       JZ Skip1
//       MOV AH,0
//       INT 16h
//       MOV BYTE PTR scan,AH
//        // MOV BYTE PTR ascii,AL 
//    } //asm
//Skip1:;
//
//    // Make sure to store only relavant keys for the game.
//    if (((scan == UP_ARROW) ||
//        (scan == DOWN_ARROW) ||
//        (scan == LEFT_ARROW) ||
//        (scan == RIGHT_ARROW) ||
//        (scan == ESCAPE_KEY))
//        && (tail < ARR_SIZE - 1))
//    {
//        entered_ascii_codes[++tail] = scan; // Store the scan code directly
//    }
//    if (scan == ESCAPE_KEY)
//    {
//        print_thank_you_message_screen();
//    }
//}

//void set_keyboard_handler(void)
//{
//    // Save the address of the old keyboard interrupt handler.
//    old_keyboard_handler = getvect(KEYBOARD_HARDWARE_INTERRUPT);
//
//    // Set the new keyboard interrupt handler.
//    setvect(KEYBOARD_HARDWARE_INTERRUPT, new_keyboard_handler);
//}



//! NOT IN USE CURRENTLY, WILL BE NEEDED FOR REFACTORING
// Restore the original keyboard interrupt handler
void restore_keyboard_handler()
{
	setvect(KEYBOARD_HARDWARE_INTERRUPT, old_keyboard_handler); 
	asm { CLI } // Disable incomming interrupts
}


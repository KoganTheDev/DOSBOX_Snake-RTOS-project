#pragma once

#ifndef KEYBOA~1_H
#define KEYBOA~1_H

#include "game_m~1.h"

#define KEYBOARD_HARDWARE_INTERRUPT 0x09

// Enumeration for different keyboard scan codes
enum keyboard_scan_codes
{
	ESCAPE_KEY = 1,
	UP_ARROW = 72,
	LEFT_ARROW = 75,
	RIGHT_ARROW = 77,
	DOWN_ARROW = 80
};

extern void interrupt (*old_keyboard_handler)(void); // Pointer to the old keyboard interrupt handler

extern char entered_ascii_codes[ARR_SIZE]; // Buffer for raw ASCII codes from keyboard
extern int tail; // Index of the last entered ASCII code

// This is a custom keyboard interrupt handler.
// It`s called every time a key is pressed. It reads the scan code and ASCII code of the key pressed without consuming it.
// It then stores the ASCII code in the `entered_ascii_codes` buffer if a valid key was pressed and the buffer isn't full.
void interrupt new_keyboard_handler();

void set_keyboard_handler(); // Initial step, connects the old handler with the new one
void restore_keyboard_handler(); // Restore the original keyboard interrupt handler
int get_next_key_from_queue(); // CRITICAL: used to get the next keyboard hit from the ISRQ while making sure there is no race condition

#endif

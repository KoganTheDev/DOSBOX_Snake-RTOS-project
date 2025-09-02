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

void set_keyboard_handler(void);
void restore_keyboard_handler(void);


#endif

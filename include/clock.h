//// This header and C file are responsible on using the clock for the game
#pragma once

#ifndef CLOCK_H
#define CLOCK_H

#include <dos.h>

#define CLOCK_HARDWARE_INTERRUPT 0x08

extern volatile int countdown_seconds;
extern volatile int countdown_running;
extern int level_time; // Total time for the level in seconds

// Initializes the PIT to generate interrupts for the countdown timer.
void set_clock_handler();

// Uninstalls the custom clock interrupt handler.
void restore_clock_handler();

// Starts the countdown timer for the current level.
void start_countdown();

// Gets the remaining time on the countdown timer.
int get_remaining_time();

#endif

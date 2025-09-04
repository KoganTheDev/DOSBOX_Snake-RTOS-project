// This header and C file are responsible on using the clock for the game
#pragma once

#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>

//! Possible improvements: use RTC interrupts instead of the time header
extern time_t start_time;

//! move level into the difficult manager(handler) when we start write a difficulty option
extern int level;

#endif 

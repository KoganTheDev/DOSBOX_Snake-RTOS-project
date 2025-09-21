#include "clock.h"

#define TIMER_TICKS_PER_SEC 18 // The number of timer ticks per second (approximately 18.2)

void interrupt (*old_clock_handler)();

volatile int countdown_seconds = 0;
volatile int countdown_running = 0;

int level_time = 60; // Default time for the level (e.g. 60 seconds)

// Counter for PIT ticks to track one second
volatile static int timer_tick_counter = 0;

// Custom Interrupt Service Routine (ISR) for the timer.
// This function is called by the hardware interrupt every 1/18.2 seconds.
// It decrements the countdown timer every second.
void interrupt new_clock_handler()
{
    // Acknowledge interrupt to the PIC, sending EOI (End Of Interrupt)
    outportb(0x20, 0x20);

    // Increment the tick counter.
    timer_tick_counter++;

    // Check if one second has passed.
    if (timer_tick_counter >= TIMER_TICKS_PER_SEC)
    {
        timer_tick_counter = 0; // Reset tick counter

        // Only decrement if the countdown is running and not at zero.
        if (countdown_running && countdown_seconds > 0)
        {
            countdown_seconds--;
        }
    }
}

void set_clock_handler()
{
    // Get the address of the old timer ISR (Interrupt CLOCK_HARDWARE_INTERRUPT)
    old_clock_handler = getvect(CLOCK_HARDWARE_INTERRUPT);

    // Set our custom ISR as the new handler for Interrupt CLOCK_HARDWARE_INTERRUPT
    setvect(CLOCK_HARDWARE_INTERRUPT, new_clock_handler);
}

void restore_clock_handler()
{
    // Restore the old ISR to clean up the system.
    setvect(CLOCK_HARDWARE_INTERRUPT, old_clock_handler);
}

void start_countdown()
{
    countdown_seconds = level_time; // Set the initial countdown time
    countdown_running = 1;         // Start the countdown
}

int get_remaining_time()
{
    return countdown_seconds;
}

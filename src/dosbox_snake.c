#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <conio.h>

#include "game_m~1.h"
#include "keyboa~1.h"
#include "screens.h"
#include "food.h"
#include "music.h"
#include "snake.h"
#include "score.h"
#include "screens.h"

void main()
{
    print_welcome_message();
    clear_display_draft(); // Clear the display draft at the start

    set_keyboard_handler();    

    while (1)
    {
        receiver(); // Get and process user input from the interrupt handler.
        updater(); // Update game logic and render the screen draft.

        // Sleep for a short duration to control the game speed.
        delay(game_speed); // Equivalent to sleep(1) in seconds.
    }
}

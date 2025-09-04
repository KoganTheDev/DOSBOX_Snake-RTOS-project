#include "screens.h"
#include "music.h"
#include "screens.h"
#include "score.h"

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

void print_thank_you_message_screen()

{
    restore_keyboard_handler();
    clrscr(); // Clear the console (Turbo C function)

    printf("\n\n\n");
    printf("                *****************************************************\n");
    printf("                *                                                   *\n");
    printf("                *        THANK YOU FOR PLAYING DOSBOX SNAKE!        *\n");
    printf("                *                                                   *\n");
    printf("                *****************************************************\n");
    printf("\n\n");
    printf("                Press any key to exit...\n");

    play_thank_you_message_melody(); // Play a simple melody until a key is pressed

    clrscr();
    exit(0);
}

void print_game_over_screen()
{
    restore_keyboard_handler();
    clrscr();

    printf("\n\n\n");
    printf("                *****************************************************\n");
    printf("                *                                                   *\n");
    printf("                *                     GAME OVER!                    *\n");
    printf("                *                                                   *\n");
    printf("                *****************************************************\n");
    printf("\n\n");
    printf("                Your final score was: %d\n", score);
    printf("\n\n");
    printf("                Press any key to exit...\n");
    
    play_game_over_melody();

    clrscr();
    exit();
}

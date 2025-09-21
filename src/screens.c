#include "screens.h"
#include "music.h"
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
    char* message; // Personalized message

    clrscr();

    // Personalized message
    if (score >= 2000) {
        message = "You're a master of the serpent! A true snake charmer!\n";
    }
    else if (score >= 1800) {
        message = "Absolutely incredible! You've reached the top tiers!\n";
    }
    else if (score >= 1500) {
        message = "A legend in the making! Unstoppable!\n";
    }
    else if (score >= 1200) {
        message = "Epic! You are a master of the grid!\n";
    }
    else if (score >= 900) {
        message = "Phenomenal! Your reflexes are unmatched!\n";
    }
    else if (score >= 600) {
        message = "A solid performance! Well done!\n";
    }
    else if (score >= 500) {
        message = "You've got the hang of it now! Keep going!\n";
    }
    else if (score >= 400) {
        message = "That's a great run! Not bad at all!\n";
    }
    else if (score >= 250) {
        message = "Good effort! You're getting better every time.\n";
    }
    else if (score >= 100) {
        message = "Keep slithering! You'll get there.\n";
    }
    else {
        message = "Each fruit is a lesson; each bite, a victory.\n";
    }

    printf("\n\n\n");
    printf("                *****************************************************\n");
    printf("                *                                                   *\n");
    printf("                *                     GAME OVER!                    *\n");
    printf("                *                                                   *\n");
    printf("                *****************************************************\n");
    printf("\n\n");
    printf("                %s\n", message);
    printf("                Your final score was: %d\n", score);
    printf("\n\n");
    printf("                Press any key to exit...\n");
    
    play_game_over_melody();

    clrscr();
    exit();
}

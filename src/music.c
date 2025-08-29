#pragma once

#include <dos.h>
#include <conio.h>

// #include "../include/music_notes.h"

// TODO: ADD gulp sound when eating food
// TODO: ADD sound effects for level up
// TODO: ADD sound effects for invalid moves (like hitting walls or timeout or (itself??) OUCH!
// TODO: ADD sound effects for game over

// Enumeration for music notes (frequencies in Hz)
enum music_notes
{
    NOTE_C4 = 262,
    NOTE_D4 = 294,
    NOTE_E4 = 330,
    NOTE_F4 = 349,
    NOTE_G4 = 392,
    NOTE_A4 = 440,
    NOTE_B4 = 494,
    NOTE_C5 = 523,
    NOTE_D5 = 587,
    NOTE_E5 = 659,
    NOTE_F5 = 698,
    NOTE_G5 = 784,
    NOTE_A5 = 880,
    NOTE_B5 = 988,
    NOTE_C6 = 1047,
    NOTE_D6 = 1175,
    NO_NOTE = 0
};

// Enumeration for note durations (in milliseconds)
enum rhythm
{
    THIRTY_SECOND_NOTE = 38,
    SIXTEENTH_NOTE = 75,
    EIGHTH_NOTE = 150,
    QUARTER_NOTE = 300,
    HALF_NOTE = 600,
    WHOLE_NOTE = 1200
};

// --- Music and sound functions ---

// Plays a sound at the given frequency for the specified duration (in milliseconds).
void playnote(int frequency, int duration)
{
    if (frequency == NO_NOTE)
    {
        delay(duration); // If it's a rest, just wait for the duration
        return;
    }
    sound(frequency); // Start playing the sound at the given frequency
    delay(duration); // Wait for the specified duration
    nosound(); // Stop playing the sound
}

// Play sound effect - used for a short burst of sound
void play_sound(int sound[][2], int notes)
{
    int i;

    for (i = 0; i < notes; i++)
    {
        playnote(sound[i][0], sound[i][1]);
    }
}

// Plays a melody until a key is pressed.
void play_melody(int melody[][2], int notes)
{
    int i = 0;
    while (!kbhit()) // Play until a key is pressed
    {
        playnote(melody[i % notes][0], melody[i % notes][1]); // Cycle through the notes
        i++;
    }
}

// Plays a melody for the welcome screen
void play_welcome_message_melody()
{
    // Welcome screen melody
    int melody[][2] = {
        {NOTE_D5, QUARTER_NOTE}, {NOTE_G5, EIGHTH_NOTE}, {NOTE_A5, EIGHTH_NOTE},
        {NOTE_B5, EIGHTH_NOTE}, {NOTE_G5, SIXTEENTH_NOTE}, {NO_NOTE, SIXTEENTH_NOTE},
        {NOTE_E5, EIGHTH_NOTE}, {NOTE_C5, EIGHTH_NOTE}, {NOTE_D5, EIGHTH_NOTE},
        {NOTE_C5, EIGHTH_NOTE}, {NOTE_A4, EIGHTH_NOTE}, {NOTE_B4, EIGHTH_NOTE},

        {NOTE_D5, QUARTER_NOTE}, {NOTE_G5, EIGHTH_NOTE}, {NOTE_A5, EIGHTH_NOTE},
        {NOTE_B5, EIGHTH_NOTE}, {NOTE_G5, SIXTEENTH_NOTE}, {NO_NOTE, SIXTEENTH_NOTE},
        {NOTE_E5, EIGHTH_NOTE}, {NOTE_C5, EIGHTH_NOTE}, {NOTE_D5, EIGHTH_NOTE},
        {NOTE_C5, EIGHTH_NOTE}, {NOTE_A4, EIGHTH_NOTE}, {NOTE_G4, EIGHTH_NOTE},

        {NOTE_A4, SIXTEENTH_NOTE}, {NOTE_A4, SIXTEENTH_NOTE},
        {NOTE_A4, SIXTEENTH_NOTE}, {NOTE_A4, SIXTEENTH_NOTE},
        {NOTE_A4, SIXTEENTH_NOTE}, {NOTE_A4, SIXTEENTH_NOTE},
        {NOTE_C5, SIXTEENTH_NOTE}, {NOTE_E5, SIXTEENTH_NOTE},
        {NOTE_C6, SIXTEENTH_NOTE}, {NOTE_B5, SIXTEENTH_NOTE},
        {NOTE_C6, SIXTEENTH_NOTE}, {NOTE_D6, SIXTEENTH_NOTE},
        {NOTE_C6, SIXTEENTH_NOTE}, {NOTE_B5, SIXTEENTH_NOTE},
        {NOTE_A5, SIXTEENTH_NOTE}, {NOTE_G5, SIXTEENTH_NOTE},
        {NOTE_E5, SIXTEENTH_NOTE}, {NOTE_C5, EIGHTH_NOTE},
        {NOTE_A4, HALF_NOTE}, {NO_NOTE, WHOLE_NOTE}
    };
    int notes = sizeof(melody) / sizeof(melody[0]);
	play_melody(melody, notes);
}

// Plays a melody for the screen that is displayed when pressing ESC
void play_thank_you_message_melody()
{
    // Soothing end-game melody
    int melody[][2] = {
        {NOTE_C5, HALF_NOTE}, {NO_NOTE, EIGHTH_NOTE},
        {NOTE_E5, QUARTER_NOTE}, {NOTE_D5, QUARTER_NOTE},
        {NOTE_G4, HALF_NOTE}, {NO_NOTE, EIGHTH_NOTE},
        {NOTE_A4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE},
        {NOTE_E4, HALF_NOTE}, {NO_NOTE, EIGHTH_NOTE},
        {NOTE_C5, HALF_NOTE}, {NO_NOTE, QUARTER_NOTE},
        {NOTE_G4, HALF_NOTE}, {NO_NOTE, HALF_NOTE}
    };
    int notes = sizeof(melody) / sizeof(melody[0]);
	play_melody(melody, notes);
}

void play_game_over_melody()
{
    // A peaceful, happy melody for the game over screen.
    int melody[][2] = {
        {NOTE_G5, QUARTER_NOTE},
        {NOTE_E5, QUARTER_NOTE},
        {NOTE_C5, HALF_NOTE},
        {NOTE_A4, QUARTER_NOTE},
        {NOTE_C5, HALF_NOTE}
    };
    int notes = sizeof(melody) / sizeof(melody[0]);
    play_melody(melody, notes);
}

// Play a sound that is used when the snake eats the fruit
void play_snake_eat_food_sound()
{
	// Snake eating food sound effect
    int sound[][2] = {
		{NOTE_C5, THIRTY_SECOND_NOTE}, {NOTE_A4, THIRTY_SECOND_NOTE},
		{NOTE_G4, THIRTY_SECOND_NOTE}, {NOTE_E4, THIRTY_SECOND_NOTE},
        {NOTE_C4, THIRTY_SECOND_NOTE}
    };
	int notes = sizeof(sound) / sizeof(sound[0]);
	play_sound(sound, notes);
}

// Plays a sound that indicates a level up, making the game harder
void play_level_up_sound()
{
    int sound[][2] = {
        {NOTE_C5, EIGHTH_NOTE}, {NOTE_E5, EIGHTH_NOTE},
        {NOTE_G5, EIGHTH_NOTE}, {NOTE_C6, EIGHTH_NOTE}
    };
    int notes = sizeof(sound) / sizeof(sound[0]);
    play_sound(sound, notes);
}

// TODO: If we decide too, we must not allow the snake to collapse into itself, and if it does play this sound
// Plays a sound when the snake hits a wall
void play_snake_hits_a_wall_sound()
{
    int sound[][2] = {
        {NOTE_E4, SIXTEENTH_NOTE},
        {NOTE_F4, SIXTEENTH_NOTE},
        {NO_NOTE, THIRTY_SECOND_NOTE} // A very short pause
    };
    int notes = sizeof(sound) / sizeof(sound[0]);
	play_sound(sound, notes);
}

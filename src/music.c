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

void play_melody(int melody[][2], int notes)
{
    int i = 0;
    while (!kbhit()) // Play until a key is pressed
    {
        playnote(melody[i % notes][0], melody[i % notes][1]); // Cycle through the notes
        i++;
    }
}

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

void play_thank_you_message_melody(void)
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

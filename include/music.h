// This header and C file are responsible on the sound design of the game
#pragma once

#ifndef MUSIC_H
#define MUSIC_H

#include <dos.h>
#include <conio.h>

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

void playnote(int frequency, int duration); // Plays a sound at the given frequency for the specified duration (in milliseconds).
void play_sound(int sound[][2], int notes); // Play sound effect - used for a short burst of sound
void play_melody(int sound[][2], int notes); // Plays a melody until a key is pressed.
void play_welcome_message_melody(); // Plays a melody for the welcome screen
void play_thank_you_message_melody(); // Plays a melody for the screen that is displayed when pressing ESC
void play_game_over_melody(); // Plays a melody for the game over screen
void play_snake_eat_food_sound(); // Play a sound that is used when the snake eats the fruit
void play_level_up_sound(); // Plays a sound that indicates a level up, making the game harder

// Plays a sound when the snake hits a wall, hits a border, collides with its body segments or moves into itself
void play_snake_dies_sound();

#endif
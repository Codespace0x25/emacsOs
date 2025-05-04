#pragma once
#include <stdint.h>
#include "PC_Speaker.h"

typedef enum {
    NOTE_A = 440, // Frequency for A4 in Hz
    NOTE_B = 493, // Frequency for B4 in Hz
    NOTE_C = 261, // Frequency for C4 in Hz
    NOTE_D = 293, // Frequency for D4 in Hz
    NOTE_E = 329, // Frequency for E4 in Hz
    NOTE_F = 349, // Frequency for F4 in Hz
    NOTE_G = 392, // Frequency for G4 in Hz
} Note;

typedef struct {
    Note note;
    uint32_t duration;
    uint32_t rest;
} MusicalNote;

void play_note(MusicalNote note);

void play_melody(MusicalNote* melody, uint32_t length);

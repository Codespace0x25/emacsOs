#pragma once
#include <stdint.h>
#include "PC_Speaker.h"

#define QN 500  // Quarter note
#define EN 250  // Eighth note
#define HN 1000 // Half note
#define SN 125  // Sixteenth note
#define REST 50 // Gap between notes

typedef enum {
    NOTE_C3  = 130,
    NOTE_CS3 = 138,
    NOTE_D3  = 146,
    NOTE_DS3 = 155,
    NOTE_E3  = 164,
    NOTE_F3  = 174,
    NOTE_FS3 = 185,
    NOTE_G3  = 196,
    NOTE_GS3 = 207,
    NOTE_A3  = 220,
    NOTE_AS3 = 233,
    NOTE_B3  = 246,

    NOTE_C4  = 261,
    NOTE_CS4 = 277,
    NOTE_D4  = 293,
    NOTE_DS4 = 311,
    NOTE_E4  = 329,
    NOTE_F4  = 349,
    NOTE_FS4 = 370,
    NOTE_G4  = 392,
    NOTE_GS4 = 415,
    NOTE_A4  = 440,
    NOTE_AS4 = 466,
    NOTE_B4  = 493,

    NOTE_C5  = 523,
    NOTE_CS5 = 554,
    NOTE_D5  = 587,
    NOTE_DS5 = 622,
    NOTE_E5  = 659,
    NOTE_F5  = 698,
    NOTE_FS5 = 740,
    NOTE_G5  = 784,
    NOTE_GS5 = 830,
    NOTE_A5  = 880,
    NOTE_AS5 = 932,
    NOTE_B5  = 987,

    NOTE_C6  = 1046,
    NOTE_CS6 = 1108,
    NOTE_D6  = 1174,
    NOTE_DS6 = 1244,
    NOTE_E6  = 1318,
    NOTE_F6  = 1396,
    NOTE_FS6 = 1480,
    NOTE_G6  = 1568,
    NOTE_GS6 = 1661,
    NOTE_A6  = 1760,
    NOTE_AS6 = 1864,
    NOTE_B6  = 1975,
} Note;

typedef struct {
    Note note;
    uint32_t duration;
    uint32_t rest;
} MusicalNote;

void play_note(MusicalNote note);

void play_melody(MusicalNote* melody, uint32_t length);

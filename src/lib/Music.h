#pragma once
#include <stdint.h>
#include "PC_Speaker.h"

#define QN 500  // Quarter note
#define EN 250  // Eighth note
#define HN 1000 // Half note
#define SN 125  // Sixteenth note
#define REST 50 // Gap between notes

typedef enum {
    // Octave 0
    NOTE_C0  = 16,
    NOTE_CS0 = 17,
    NOTE_D0  = 18,
    NOTE_DS0 = 19,
    NOTE_E0  = 21,
    NOTE_F0  = 22,
    NOTE_FS0 = 23,
    NOTE_G0  = 25,
    NOTE_GS0 = 26,
    NOTE_A0  = 28,
    NOTE_AS0 = 29,
    NOTE_B0  = 31,

    // Octave 1
    NOTE_C1  = 33,
    NOTE_CS1 = 35,
    NOTE_D1  = 37,
    NOTE_DS1 = 39,
    NOTE_E1  = 41,
    NOTE_F1  = 44,
    NOTE_FS1 = 46,
    NOTE_G1  = 49,
    NOTE_GS1 = 52,
    NOTE_A1  = 55,
    NOTE_AS1 = 58,
    NOTE_B1  = 62,

    // Octave 2
    NOTE_C2  = 65,
    NOTE_CS2 = 69,
    NOTE_D2  = 73,
    NOTE_DS2 = 78,
    NOTE_E2  = 82,
    NOTE_F2  = 87,
    NOTE_FS2 = 93,
    NOTE_G2  = 98,
    NOTE_GS2 = 104,
    NOTE_A2  = 110,
    NOTE_AS2 = 117,
    NOTE_B2  = 123,

    // Existing: Octave 3 to 6 (already given)

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

    // Octave 7
    NOTE_C7  = 2093,
    NOTE_CS7 = 2217,
    NOTE_D7  = 2349,
    NOTE_DS7 = 2489,
    NOTE_E7  = 2637,
    NOTE_F7  = 2793,
    NOTE_FS7 = 2960,
    NOTE_G7  = 3136,
    NOTE_GS7 = 3322,
    NOTE_A7  = 3520,
    NOTE_AS7 = 3729,
    NOTE_B7  = 3951,

    // Octave 8
    NOTE_C8  = 4186,
    NOTE_CS8 = 4435,
    NOTE_D8  = 4699,
    NOTE_DS8 = 4978,
    NOTE_E8  = 5274,
    NOTE_F8  = 5587,
    NOTE_FS8 = 5920,
    NOTE_G8  = 6272,
    NOTE_GS8 = 6645,
    NOTE_A8  = 7040,
    NOTE_AS8 = 7458,
    NOTE_B8  = 7902,
} Note;

typedef struct {
    Note note;
    uint32_t duration;
    uint32_t rest;
} MusicalNote;

void play_note(MusicalNote note);

void play_melody(MusicalNote* melody, uint32_t length);

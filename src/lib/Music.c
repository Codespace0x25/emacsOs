#include "Music.h"
#include "PC_Speaker.h"

void play_note(MusicalNote note) {
    pc_speaker_on(note.note);
    pc_beep(note.note, note.duration);
    pc_speaker_off();
    delay(note.rest);
}

void play_melody(MusicalNote* melody, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        play_note(melody[i]);
        delay(5000);
    }
}

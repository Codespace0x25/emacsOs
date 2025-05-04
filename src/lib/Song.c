// Song.c
#include "Song.h"
#include "Screen.h"
#include "std/string.h"

static const MusicalNote TheImperialMarch[] = {
    {NOTE_A4, 5000, 5000},
    {NOTE_A4, 5000, 5000},
    {NOTE_A4, 5000, 5000},
    {NOTE_F4, 3500, 5000},
    {NOTE_C5, 1500, 5000},
    {NOTE_A4, 10000, 10000},
    {NOTE_F4, 3500, 5000},
    {NOTE_C5, 1500, 5000},
    {NOTE_A4, 10000, 10000},
    {NOTE_E5, 5000, 5000}, 
    {NOTE_E5, 5000, 5000}, 
    {NOTE_E5, 5000, 5000},
    {NOTE_F5, 3500, 5000},
    {NOTE_C5, 1500, 5000},
    {NOTE_GS4, 10000, 10000},
    {NOTE_F4, 3500, 5000},
    {NOTE_C5, 1500, 5000},
    {NOTE_A4, 10000, 10000},
    {NOTE_A5, 5000, 5000}, 
    {NOTE_A4, 3500, 5000},
    {NOTE_A4, 1500, 5000},
    {NOTE_A5, 5000, 5000},
    {NOTE_GS5, 2500, 5000},
    {NOTE_G5, 2500, 5000},
    {NOTE_FS5, 1250, 5000},
    {NOTE_F5, 1250, 5000},
    {NOTE_FS5, 2500, 5000},
    {NOTE_AS4, 2500, 5000},
    {NOTE_DS5, 5000, 5000},
    {NOTE_D5, 2500, 5000},
    {NOTE_CS5, 2500, 5000},
    {NOTE_C5, 1250, 5000},
    {NOTE_B4, 1250, 5000},
    {NOTE_C5, 2500, 5000},
    {NOTE_F4, 1250, 5000},
    {NOTE_GS4, 5000, 5000},
    {NOTE_F4, 3750, 5000},
    {NOTE_A4, 1250, 10000}
};

// A helper function to calculate the size of the song array
#define SONG_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

// Define the song registry with the correct size
static const Song songRegistry[] = {
    {"imperial", TheImperialMarch, SONG_SIZE(TheImperialMarch)}
};

// Song playback function
void playSong(MusicalNote *song, size_t song_length) {
    play_melody(song, song_length);
}

const Song* find_song_by_name(const char *name) {
    for (size_t i = 0; i < sizeof(songRegistry) / sizeof(songRegistry[0]); i++) {
        if (strcmp(songRegistry[i].name, name) == 0) {
            return &songRegistry[i];
        }
    }
    return NULL;
}

void playSong_by_name(const char *name) {
  const  Song* song = find_song_by_name(name);
  playSong(song->notes, song->length);
}

void cmd_music(int argc, char **argv) {
    if (argc < 2) {
        putS("Please provide the name of the song\n");
        return;
    }

    const Song *song = find_song_by_name(argv[1]);
    if (!song) {
        putS("Song not found\n");
        return;
    }

    playSong(song->notes, song->length);
}

// Song.c
#include "Song.h"
#include "Music.h"
#include "Screen.h"
#include "std/string.h"
#include <string.h>

static const MusicalNote TheImperialMarch[] = {
    {NOTE_A3, 5000, 5000},  {NOTE_A3, 5000, 5000},  {NOTE_A3, 5000, 5000},
    {NOTE_F3, 3500, 5000},  {NOTE_C4, 1500, 5000},  {NOTE_A3, 10000, 10000},
    {NOTE_F3, 3500, 5000},  {NOTE_C4, 1500, 5000},  {NOTE_A3, 10000, 10000},
    {NOTE_E4, 5000, 500},   {NOTE_E4, 5000, 5000},  {NOTE_E4, 5000, 5000},
    {NOTE_F4, 3500, 500},   {NOTE_C4, 1500, 5000},  {NOTE_GS3, 10000, 10000},
    {NOTE_F3, 3500, 5000},  {NOTE_C4, 1500, 5000},  {NOTE_A3, 10000, 10000},
    {NOTE_A4, 5000, 5000},  {NOTE_A3, 3500, 5000},  {NOTE_A3, 1500, 5000},
    {NOTE_A4, 5000, 5000},  {NOTE_GS4, 2500, 5000}, {NOTE_G4, 2500, 5000},
    {NOTE_FS4, 1250, 5000}, {NOTE_F4, 1250, 5000},  {NOTE_FS4, 2500, 5000},
    {NOTE_AS3, 2500, 5000}, {NOTE_DS4, 5000, 5000}, {NOTE_D4, 2500, 5000},
    {NOTE_CS4, 2500, 5000}, {NOTE_C4, 1250, 5000},  {NOTE_B3, 1250, 5000},
    {NOTE_C4, 2500, 5000},  {NOTE_F3, 1250, 5000},  {NOTE_GS3, 5000, 5000},
    {NOTE_F3, 3750, 5000},  {NOTE_A3, 1250, 10000},
};

static const MusicalNote sna[] = {
    {NOTE_E2, 55000, 2590 * 2}, {NOTE_E2, 35000, 1590}, {NOTE_G2, 35000, 1590},
    {NOTE_E2, 35000, 1590},     {NOTE_D2, 55000, 5590}, {NOTE_C2, 35000, 1590},

    {1, 1, 5590 * 2}, // this must be 1 as 0 would crash

    {NOTE_E2, 55000, 2590 * 2}, {NOTE_E2, 35000, 1590}, {NOTE_G2, 35000, 1590},
    {NOTE_E2, 35000, 1590},     {NOTE_D2, 55000, 5590}, {NOTE_C2, 35000, 1590},
};

#define SONG_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define REGISTER_SONG(CALLNAME, SONGNAME, SONG)                                \
  {CALLNAME, SONGNAME, SONG, SONG_SIZE(SONG)},

static const Song songRegistry[] = {
    REGISTER_SONG("imperial", "The Imperial March", TheImperialMarch)
        REGISTER_SONG("sevenNationArmy", "Seven Nation Army", sna)};

void playSong(MusicalNote *song, size_t song_length) {
  play_melody(song, song_length);
}

const Song *find_song_by_name(const char *name) {
  for (size_t i = 0; i < sizeof(songRegistry) / sizeof(songRegistry[0]); i++) {
    if (strcmp(songRegistry[i].name, name) == 0) {
      return &songRegistry[i];
    }
  }
  return NULL;
}

void playSong_by_name(const char *name) {
  const Song *song = find_song_by_name(name);
  playSong((MusicalNote *)song->notes, song->length);
}

void cmd_music(int argc, char **argv) {
  if (argc < 2) {
    putS("Please provide the name of the song or use --list to see available "
         "songs\n");
    return;
  }

  if (strcmp(argv[1], "--list") == 0) {
    printf("List of songs:\n");
    printf("call Name | song name\n");
    for (size_t i = 0; i < sizeof(songRegistry) / sizeof(songRegistry[0]);
         i++) {
      printf("\t%s | %s \n", songRegistry[i].name, songRegistry[i].song_name);
    }
  } else {
    const Song *song = find_song_by_name(argv[1]);
    if (!song) {
      putS("Song not found\n");
      return;
    }

    playSong((MusicalNote *)song->notes, song->length);
  }
}

// Song.h
#pragma once
#include "Music.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    const char *name;
    const char *song_name;
    const MusicalNote *notes;
    size_t length;
} Song;

void playSong(MusicalNote *song, size_t song_length);

void playSong_by_name(const char *name);

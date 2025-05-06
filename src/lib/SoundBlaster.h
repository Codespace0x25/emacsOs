#pragma once
#include <stdint.h>
#include <stddef.h>
int sb16_reset_dsp();

// Set sample rate (output)
void sb16_set_sample_rate(uint16_t hz);
void generate_square_wave(uint8_t* buffer, size_t len, int freq, int rate);

// Set up DMA channel 1
void sb16_setup_dma(uint8_t* buffer, size_t length);

// Begin playback
void sb16_play(uint16_t length);

// --- Full Initialization ---
int sb16_init();

// --- Call to Play Sound ---
void sb16_test_play();

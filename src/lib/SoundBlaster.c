#include <stdint.h>
#include <stddef.h>
#include "SoundBlaster.h"
#include "DataPort.h"

#define SB_BASE         0x220
#define DSP_RESET       (SB_BASE + 0x6)
#define DSP_READ        (SB_BASE + 0xA)
#define DSP_WRITE       (SB_BASE + 0xC)
#define DSP_WRITE_STAT  (SB_BASE + 0xC)
#define DSP_READ_STAT   (SB_BASE + 0xE)
#define DSP_INT_ACK     (SB_BASE + 0xE)

#define DMA_CHANNEL     1
#define DMA_PAGE        0x83  // Page register for DMA channel 1
#define DMA_MASK_REG    0x0A
#define DMA_MODE_REG    0x0B
#define DMA_CLEAR_FF    0x0C
#define DMA_ADDR_REG    0x02
#define DMA_COUNT_REG   0x03

#define SAMPLE_RATE     22050
#define PCM_LEN         ((SAMPLE_RATE * 500) / 1000) // 0.5s
static uint8_t pcm[PCM_LEN] __attribute__((aligned(4096))); // DMA buffer



// Reset and detect DSP
int sb16_reset_dsp() {
    outb(DSP_RESET, 1);
    io_wait();
    outb(DSP_RESET, 0);

    for (int i = 0; i < 1000; ++i) {
        if (inb(DSP_READ_STAT) & 0x80) {
            if (inb(DSP_READ) == 0xAA)
                return 1; // Success
        }
    }
    return 0;
}

// Set sample rate (output)
void sb16_set_sample_rate(uint16_t hz) {
    outb(DSP_WRITE, 0x41);
    outb(DSP_WRITE, hz >> 8);
    outb(DSP_WRITE, hz & 0xFF);
}

// Generate square wave
void generate_square_wave(uint8_t* buffer, size_t len, int freq, int rate) {
    int samples_per_cycle = rate / freq;
    for (size_t i = 0; i < len; ++i) {
        buffer[i] = (i % samples_per_cycle < samples_per_cycle / 2) ? 0xFF : 0x00;
    }
}

// Set up DMA channel 1
void sb16_setup_dma(uint8_t* buffer, size_t length) {
    uintptr_t addr = (uintptr_t)buffer;
    uint16_t offset = addr;
    uint8_t page = addr >> 16;
    uint16_t count = length - 1;

    outb(DMA_MASK_REG, DMA_CHANNEL | 0x04); // Mask DMA
    outb(DMA_CLEAR_FF, 0x00);               // Reset FF

    outb(DMA_ADDR_REG, offset & 0xFF);
    outb(DMA_ADDR_REG, offset >> 8);
    outb(DMA_PAGE, page);

    outb(DMA_COUNT_REG, count & 0xFF);
    outb(DMA_COUNT_REG, count >> 8);

    outb(DMA_MODE_REG, 0x49); // Single mode, increment, write, chan 1
    outb(DMA_MASK_REG, DMA_CHANNEL); // Unmask
}

// Begin playback
void sb16_play(uint16_t length) {
    outb(DSP_WRITE, 0xC6); // 8-bit DMA, single cycle, output
    outb(DSP_WRITE, length & 0xFF);
    outb(DSP_WRITE, length >> 8);
}

// --- Full Initialization ---
int sb16_init() {
    if (!sb16_reset_dsp()) {
        return 0; // Failure
    }

    sb16_set_sample_rate(SAMPLE_RATE);
    generate_square_wave(pcm, PCM_LEN, 440, SAMPLE_RATE);
    sb16_setup_dma(pcm, PCM_LEN);
    return 1;
}

// --- Call to Play Sound ---
void sb16_test_play() {
    if (sb16_init()) {
        sb16_play(PCM_LEN);
    }
}

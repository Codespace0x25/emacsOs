#include "SoundBlaster.h"
#include "DataPort.h"
#include <stdio.h>


// Reset the Sound Blaster DSP
int sb_reset() {
    outb(DSP_RESET, 1);  // Assert reset
    io_wait();
    outb(DSP_RESET, 0);  // Deassert reset
    io_wait();

    // Wait for the DSP to be ready
    for (int i = 0; i < 1000; ++i) {
        if (inb(DSP_STATUS) & 0x80) {
            uint8_t dsp_id = inb(DSP_READ);  // Read DSP ID byte
            if (dsp_id == 0xAA) {
                printf("DSP reset successful\n");
                return 1;
            } else {
                // Debug: Print the DSP ID
                printf("Unexpected DSP ID: 0x%02X\n", dsp_id);
                return 0;
            }
        }
    }
    printf("Failed to initialize DSP.\n");
    return 0;
}

// Set up DMA for playback
void sb_dma_setup(uint8_t *buffer, uint16_t length) {
    uint32_t addr = (uint32_t)buffer;
    uint8_t page = (addr >> 16) & 0xFF;
    uint16_t offset = addr & 0xFFFF;

    // Debugging: Print buffer and DMA information
    printf("Setting up DMA: buffer=0x%p, length=%d, page=0x%02X, offset=0x%04X\n", buffer, length, page, offset);

    outb(0x0A, 0x05);  // Mask DMA ch1
    outb(0x0C, 0x00);  // Reset flip-flop

    outb(0x02, offset & 0xFF);    // Set low byte of the buffer offset
    outb(0x02, offset >> 8);      // Set high byte of the buffer offset
    outb(0x83, page);             // Set DMA page for ch1

    outb(0x0C, 0x00);  // Unmask DMA channel

    length--;
    outb(0x03, length & 0xFF);    // Set low byte of buffer length
    outb(0x03, length >> 8);      // Set high byte of buffer length

    outb(0x0B, 0x49);  // Single, read, channel 1
    outb(0x0A, 0x01);  // Unmask channel 1
}

// Play an 8-bit sound buffer with a given sample rate
void sb_play_8bit(uint8_t *buffer, uint16_t length, uint8_t sample_rate) {
    if (!sb_reset()) return;

    sb_dma_setup(buffer, length);

    // Set the playback time constant for the sample rate
    outb(DSP_WRITE, 0x40);                // Set time constant for frequency
    outb(DSP_WRITE, 256 - sample_rate);   // e.g. 11025Hz -> 0xAC

    // Set 8-bit PCM, mono playback
    outb(DSP_WRITE, 0xC6);                // 8-bit, mono
    outb(DSP_WRITE, (length - 1) & 0xFF);  // Set low byte of length
    outb(DSP_WRITE, (length - 1) >> 8);    // Set high byte of length
}


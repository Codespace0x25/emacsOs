#ifndef SOUND_BLASTER_H
#define SOUND_BLASTER_H

#include <stdint.h>

#define SB_BASE     0x220
#define DSP_RESET   (SB_BASE + 0x6)
#define DSP_READ    (SB_BASE + 0xA)
#define DSP_WRITE   (SB_BASE + 0xC)
#define DSP_STATUS  (SB_BASE + 0xE)

int sb_reset();
void sb_dma_setup(uint8_t *buffer, uint16_t length);
void sb_play_8bit(uint8_t *buffer, uint16_t length, uint8_t sample_rate);

#endif

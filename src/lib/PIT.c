#include "DataPort.h"
#include "Screen.h"

#define PIT_CHANNEL0_DATA  0x40
#define PIT_COMMAND        0x43
#define PIT_FREQUENCY      1193182

void pit_set_frequency(uint32_t frequency) {
    uint16_t divisor = PIT_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0_DATA, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_DATA, (uint8_t)((divisor >> 8) & 0xFF));
}

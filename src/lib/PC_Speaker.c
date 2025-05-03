#include "DataPort.h"
#include <stdint.h>

#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43
#define SPEAKRE_PORT      0x61
#define PIT_FREQUENCY     1193182

void pc_speaker_on(uint32_t frek){
  uint16_t divisor = PIT_FREQUENCY/frek;
  outb(PIT_COMMAND_PORT, 0xB6);
  outb(PIT_CHANNEL2_PORT,divisor& 0xFF);
  outb(PIT_CHANNEL2_PORT,(divisor >> 8)& 0xFF);
  uint8_t tmp = inb(SPEAKRE_PORT);
  if (!(tmp & 3)) {
    outb(SPEAKRE_PORT, tmp | 3);
  }
}
void pc_speaker_off(){
  uint8_t tmp = inb(SPEAKRE_PORT)&0xFC;
  outb(SPEAKRE_PORT, tmp);
}

void delay(uint32_t count){
  for (volatile uint32_t i = 0;i < count*1000;i++) ;
}

void pc_beep(uint32_t frek,uint32_t dur){
  pc_speaker_on(frek);
  delay(dur);
  pc_speaker_off();
}

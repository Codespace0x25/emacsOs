#include "DataPort.h"

void poweroff(){
  outw(0x604,0x2000); // qemu
  outw(0xB04,0x2000); // booches
  outw(0x4004,0x3400); // vmbox
  while (1) {
    __asm__  __volatile__("hlt");
  }
}

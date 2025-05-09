#include "lib/ATA.h"
#include "lib/Allocator.h"
#include "lib/Com1.h"
#include "lib/DataPort.h"
#include "lib/Def.h"
#include "lib/EXT2.h"
#include "lib/FrameBuffer.h"
#include "lib/GDT.h"
#include "lib/Music.h"
#include "lib/PCI.h"
#include "lib/PC_Speaker.h"
#include "lib/PIT.h"
#include "lib/Page.h"
#include "lib/RTC.h"
#include "lib/Screen.h"
#include "lib/Song.h"
#include "lib/SoundBlaster.h"
#include "lib/Static.h"
#include "lib/idt.h"
#include "lib/std/string.h"
#include <stdint.h>

#define HLT                                                                    \
  while (1)                                                                    \
    ;
#define HEAP_SIZE (1024 * 100)
static uint8_t heap[HEAP_SIZE];

void kernel_main() {
  Screen_Clear();
  GDT_Install();
  PIC_remap();
  paging_init();
  serial_init();
  idt_install();
  sb16_init();
  pit_set_frequency(100);
  rtc_enable_periodic_interrupt();
  allocator_init(heap, HEAP_SIZE);
  pci_scan_bus();
  ata_initialize();

  delay(10000 * 5);
  Screen_Clear();
  serial_printf("Kernal: kernel inited");

  Screen_SetColor(MAGENTA, BLACK);
  printf("welcom to Vlipos\n");
  Screen_SetColor(YELLOW, BLACK);
  printf("heep size, %d and it starts at, %x\n", HEAP_SIZE, &heap);
  Screen_SetColor(BROWN, BLACK);
  printf("you are loged in as System, you are running at kernel level\n");
  Screen_DefaltColor();
  if (!parse_mbr(0)) {
    Kerrror("EXT2: init failed.\n");
  } else {
    Kerrror("EXT2: ready for file operations.\n");
  }

  putPS1();

  HLT
}

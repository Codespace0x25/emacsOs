#include "lib/Allocator.h"
#include "lib/Com1.h"
#include "lib/DataPort.h"
#include "lib/GDT.h"
#include "lib/Music.h"
#include "lib/PC_Speaker.h"
#include "lib/Song.h"
#include "lib/PIT.h"
#include "lib/Page.h"
#include "lib/RTC.h"
#include "lib/Screen.h"
#include "lib/Static.h"
#include "lib/idt.h"
#include "lib/std/string.h"
#include <stdint.h>

#define HLT                                                                    \
  while (1)                                                                    \
    ;
#define HEAP_SIZE (1024 * 1024) * 5 // 1MB for heap space
static uint8_t heap[HEAP_SIZE];     // Allocate space for the heap

void kernel_main(void) {
  Screen_Clear();
  GDT_Install();
  PIC_remap();
  paging_init();
  serial_init();
  idt_install();
  pit_set_frequency(100);
  rtc_enable_periodic_interrupt();
  allocator_init(heap, HEAP_SIZE);
  playSong_by_name("imperial");
  putS("if you hard a sound the pc speacker works\n");

  serial_putS("kernal loaded");
  putS(GetUserName());
  putS("> ");

  HLT
}

#include "lib/Allocator.h"
#include "lib/Com1.h"
#include "lib/DataPort.h"
#include "lib/GDT.h"
#include "lib/Music.h"
#include "lib/PC_Speaker.h"
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

MusicalNote melody[] = {
    {NOTE_A, 1000, 100},    // A4 for 1s, short rest
    {NOTE_A,  5000, 250},    // A4 for 0.5s, mid rest
    {NOTE_E,  7500, 250},    // E4 for 0.75s, pause
    {NOTE_G,  5000, 500},    // G4 for 0.5s, longer rest
    {NOTE_F,  6000, 300},    // F4 for 0.6s, pause
    {NOTE_A, 1000, 100},    // A4 for 1s, short rest
    {NOTE_A,  5000, 250},    // A4 for 0.5s, mid rest
    {NOTE_E,  7500, 250},    // E4 for 0.75s, pause
    {NOTE_G,  5000, 500},    // G4 for 0.5s, longer rest
    {NOTE_F,  6000, 300},    // F4 for 0.6s, pause
    {NOTE_A, 1000, 100},    // A4 for 1s, short rest
    {NOTE_A,  5000, 250},    // A4 for 0.5s, mid rest
    {NOTE_E,  7500, 250},    // E4 for 0.75s, pause
    {NOTE_G,  5000, 500},    // G4 for 0.5s, longer rest
    {NOTE_F,  6000, 300},    // F4 for 0.6s, pause
    {NOTE_A, 1000, 100},    // A4 for 1s, short rest
    {NOTE_A,  5000, 250},    // A4 for 0.5s, mid rest
    {NOTE_E,  7500, 250},    // E4 for 0.75s, pause
    {NOTE_G,  5000, 500},    // G4 for 0.5s, longer rest
    {NOTE_F,  6000, 300},    // F4 for 0.6s, pause
};

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

  play_melody(melody, sizeof(melody) / sizeof(melody[0]));
  putS("if you hard a sound the pc speacker works\n");

  serial_putS("kernal loaded");
  putS(GetUserName());
  putS("> ");

  HLT
}

#include "GDT.h"
#include <stdint.h>
#include "Screen.h"

struct GDTEntry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));

struct GDTPtr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

static struct GDTEntry gdt[3];
static struct GDTPtr gdt_ptr;

extern void GDT_Load(struct GDTPtr*);

static void GDT_SetEntry(int idx, uint32_t base, uint32_t limit, uint8_t access,
                         uint8_t gran) {
  gdt[idx].limit_low = (limit & 0xFFFF);
  gdt[idx].base_low = (base & 0xFFFF);
  gdt[idx].base_middle = (base >> 16) & 0xFF;
  gdt[idx].access = access;
  gdt[idx].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
  gdt[idx].base_high = (base >> 24) & 0xFF;
}

void GDT_Install() {
  gdt_ptr.limit = sizeof(gdt) - 1;
  gdt_ptr.base = (uint32_t)&gdt;

  GDT_SetEntry(0, 0, 0, 0, 0);                        // Null segment
  GDT_SetEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);          // 32-bit code segment
  GDT_SetEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);          // 32-bit data segment

  GDT_Load(&gdt_ptr);

  putS("[");
  Screen_SetColor(GREEN, BLACK);
  putS("yes");
  Screen_SetColor(WHITE, BLACK);
  putS("] ");
  putS("init GDT\n");
}

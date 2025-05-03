#pragma once
#include <stdint.h>

#define IDT_NUM_VECTORS 256

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern struct idt_entry idt[IDT_NUM_VECTORS];
extern struct idt_ptr idt_reg;


static char input_buffer[256];
static int buffer_index ;

void PIC_remap();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);

void idt_install();

void keyboard_handler();


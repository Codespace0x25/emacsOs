#include "idt.h"
#include "CommandHandler.h"
#include "DataPort.h"
#include "Screen.h"
#include "Static.h"
#include <stdbool.h>
#include <stdint.h>

#define KERNEL_CODE_SEGMENT 0x08 // Assume the kernel code segment is 0x08

// Declare the IDT and IDT pointer
struct idt_entry idt[IDT_NUM_VECTORS];
struct idt_ptr idt_reg;

static char input_buffer[256];
static int buffer_index = 0;

extern void keyboard_handler_stub();   // not the C version
extern void page_fault_handler_stub(); // ASM wrapper

// The interrupt handler for keyboard interrupts (IRQ1)
// Simplified scancode to ASCII map (US QWERTY, no shift handling)

void keyboard_handler() {
  uint8_t scancode = inb(0x60);
  static bool shift_pressed = false;

  static const char scancode_ascii[] = {
      0,    27,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
      '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
      '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
      0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
      0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

  static const char scancode_shifted[] = {
      0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',  '+',
      '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{',  '}',
      '\n', 0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
      0,    '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,    '*',
      0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

  if (scancode == 0x2A || scancode == 0x36) {
    shift_pressed = true;
  } else if (scancode == 0xAA || scancode == 0xB6) {
    shift_pressed = false;
  } else if (scancode == 0x0E) {
    if (buffer_index > 0) {
      buffer_index--;
      Screen_MoveCursorBack();
      putS(" ");
      Screen_MoveCursorBack();
    }
  } else if (scancode < sizeof(scancode_ascii)) {
    char c =
        shift_pressed ? scancode_shifted[scancode] : scancode_ascii[scancode];
    if (c) {
      if (c == '\n') {
        input_buffer[buffer_index] = '\0';
        putS("\n");
        handle_command(input_buffer);
        buffer_index = 0;
        putS(GetUserName());
        putS("> ");
      } else {
        if (buffer_index < sizeof(input_buffer) - 1) {
          input_buffer[buffer_index++] = c;
          char str[2] = {c, 0};
          putS(str);
        }
      }
    }
  }

  outb(0x20, 0x20); // Send EOI to PIC
}

// Set up an entry in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector,
                  uint8_t flags) {
  idt[num].base_low = (base & 0xFFFF);
  idt[num].base_high = ((base >> 16) & 0xFFFF);
  idt[num].selector = selector;
  idt[num].always0 = 0;
  idt[num].flags = flags;
}
void PIC_remap() {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20); // Master PIC vector offset
  outb(0xA1, 0x28); // Slave PIC vector offset
  outb(0x21, 0x04); // Tell Master PIC about slave at IRQ2
  outb(0xA1, 0x02); // Tell Slave PIC its cascade identity
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0); // Unmask all IRQs
  outb(0xA1, 0x0);
  // Enable only IRQ1 (keyboard) on master PIC
  outb(0x21, 0xFD); // 1111 1101 = allow IRQ1
}

// Install the IDT
void idt_install() {
  // Initialize IDT entries

  for (int i = 0; i < IDT_NUM_VECTORS; i++) {
    idt_set_gate(i, 0, 0, 0);
  }

  // Set up the keyboard interrupt handler (IRQ1)
  idt_set_gate(0x21, (uint32_t)keyboard_handler_stub, KERNEL_CODE_SEGMENT,
               0x8E);
  idt_set_gate(14, (uint32_t)page_fault_handler_stub, 0x08, 0x8E);

  // Load the IDT
  idt_reg.limit = sizeof(struct idt_entry) * IDT_NUM_VECTORS - 1;
  idt_reg.base = (uint32_t)&idt;
  __asm__ volatile("lidt %0" : : "m"(idt_reg));
  __asm__ volatile("sti"); // Set Interrupt Flag — enables IRQs

  Screen_SetColor(WHITE, BLACK);
  putS("[");
  Screen_SetColor(GREEN, BLACK);
  putS("yes");
  Screen_SetColor(WHITE, BLACK);
  putS("] ");
  putS("init IDT\n");
}

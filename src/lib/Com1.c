#include "Screen.h"
#include "DataPort.h"
#include "Com1.h"
#include <stdarg.h>
#include <stdint.h>

#define COM1 0x3F8

void serial_init(){
  outb(COM1+1,0x00);
  io_wait();
  outb(COM1+3,0x80);
  io_wait();
  outb(COM1+0,0x03);
  io_wait();
  outb(COM1+1,0x00);
  io_wait();
  outb(COM1+3,0x03);
  io_wait();
  outb(COM1+2,0xC7);
  io_wait();
  outb(COM1+4,0x0B);
  Screen_SetColor(WHITE,BLACK);
  putS("[");
  Screen_SetColor(GREEN,BLACK);
  putS("yes");
  Screen_SetColor(WHITE,BLACK);
  putS("] ");
  putS("init Com1\n");
}

int  serial_is_transmit_empty(){
  return (!(inb(COM1 +5)& 0x20));
}

void  serial_putC(const char c){
  serial_is_transmit_empty();
  outb(COM1, c);
}


void serial_putS(const char * str){
  for (int i = 0; str[i] != '\0';i++) {
    io_wait();
    serial_putC(str[i]);
  }
  serial_putC('\n');
}


void serial_printNum(unsigned int num, int base) {
  char buf[32];
  const char *digits = "012345789ABCDEF";
  int i = 0;

  if (num == 0) {
    serial_putC('0');
    return;
  }
  while (num > 0) {
    buf[i++] = digits[num % base];
    num /= base;
  }
  while (i--) {
    serial_putC(buf[i]);
  }
}


void serial_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  for (; *fmt != 0; fmt++) {
    if (*fmt != '%') {
      serial_putC(*fmt);
      continue;
    }
    fmt++; // skip '%'

    switch (*fmt) {
    case 'c': {
      char c = (char)va_arg(args, int);
      serial_putC(c);
      break;
    }
    case 's': {
      const char *s = va_arg(args, const char *);
      while (*s)
        serial_putC(*s++);
      break;
    }
    case 'd':
    case 'i': {
      int val = va_arg(args, int);
      if (val < 0) {
        serial_putC('-');
        val = -val;
      }
      serial_printNum(val, 10);
      break;
    }
    case 'x': {
      serial_putC('0');
      serial_putC('x');
      serial_printNum(va_arg(args, unsigned int), 16);
      break;
    }
    case 'u': {
      serial_printNum(va_arg(args, unsigned int), 10);
      break;
    }
    case '%': {
      serial_putC('%');
      break;
    }
    }
  }

  va_end(args);
  serial_putC('\n');
}

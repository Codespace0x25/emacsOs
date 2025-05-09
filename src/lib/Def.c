#include "Allocator.h"
#include "Com1.h"
#include "DataPort.h"
#include "GDT.h"
#include "Music.h"
#include "PCI.h"
#include "PC_Speaker.h"
#include "PIT.h"
#include "Page.h"
#include "RTC.h"
#include "Screen.h"
#include "Song.h"
#include "SoundBlaster.h"
#include "Static.h"
#include "idt.h"
#include "std/string.h"
#include <stdarg.h>
#include <stdint.h>

#define va_start(v, l) __builtin_va_start(v, l)
#define va_arg(v, t) __builtin_va_arg(v, t)
#define va_end(v) __builtin_va_end(v)

void print_number(unsigned int num, int base);
void putC(const char c);
void Kerrror(const char *fmt, ...) {
  Screen_SetColor(RED,BLACK);
  va_list args;
  va_start(args, fmt);

  for (; *fmt != 0; fmt++) {
    if (*fmt != '%') {
      putC(*fmt);
      serial_putC(*fmt);
      continue;
    }
    fmt++; // skip '%'

    switch (*fmt) {
    case 'c': {
      char c = (char)va_arg(args, int);
      putC(c);
      serial_putC(c);
      break;
    }
    case 's': {
      const char *s = va_arg(args, const char *);
      while (*s){
        putC(*s++);
	serial_putC(*s++);
      }
      break;
    }
    case 'd':
    case 'i': {
      int val = va_arg(args, int);
      if (val < 0) {
        putC('-');
        val = -val;
      }
      print_number(val, 10);
      serial_printNum(val, 10);
      break;
    }
    case 'x': {
      putC('0');
      putC('x');
      print_number(va_arg(args, unsigned int), 16);
      serial_printNum(va_arg(args, unsigned int), 16);
      break;
    }
    case 'u': {
      print_number(va_arg(args, unsigned int), 10);
      serial_printNum(va_arg(args, unsigned int), 10);
      break;
    }
    case '%': {
      putC('%');
      serial_putC('%');
      break;
    }
    }
  }

  va_end(args);
  Screen_DefaltColor();
}

void putPS1() {
  putS(GetUserName());
  putS("> ");
}

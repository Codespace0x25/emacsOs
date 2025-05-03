#include "Screen.h"
#include "DataPort.h"
#include "Com1.h"

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

void  serial_write_char(const char c){
  serial_is_transmit_empty();
  outb(COM1, c);
}

void serial_putS(const char * str){
  for (int i = 0; str[i] != '\0';i++) {
    io_wait();
    serial_write_char(str[i]);
  }
  serial_write_char('\n');
}

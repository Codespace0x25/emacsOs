#pragma once

void serial_init();

void  serial_putC(const char c);

void serial_putS(const char * str);

void serial_printNum(unsigned int num, int base);

void serial_printf(const char *fmt, ...);

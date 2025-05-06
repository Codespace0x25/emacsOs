#pragma once
#include <stdint.h>


void int_to_string(int value, char *buffer) ;

int32_t string_to_int(const char* str);

void int_to_hex(uint32_t value, char *buf) ;

uint8_t hex_char_to_int(char c);

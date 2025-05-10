#include "transform.h"
#include <stdbool.h>
#include <stdint.h>

void int_to_string(int value, char *buffer) {
  char temp[12];
  int i = 0, is_negative = 0;

  if (value == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  }
  if (value < 0) {
    is_negative = 1;
    value = -value;
  }
  while (value > 0) {
    temp[i++] = (value % 10) + '0';
    value /= 10;
  }
  if (is_negative)
    temp[i++] = '-';
  for (int j = 0;j < i;j++) {
    buffer[j] = temp[i-j-1];
  }
  buffer[i] = '\0';
}

int32_t string_to_int(const char* str){
  int32_t result = 0;
  bool neg = false;
  while (*str == ' ' || *str == '\t' || *str == '\n') {
    str++;
  }
  if (*str=='-') {
    neg = true;
    str++;
  }else if (*str == '+') {
    str++;
  }
  while (*str >= '0' && *str <='9') {
    result = result * 10 + (*str - '0');
    str++;
  }
  return neg ? -result: result;
}
void int_to_hex(uint32_t value, char *buf) {
    static const char *hex_chars = "0123456789ABCDEF";
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 0; i < 8; ++i) {
        buf[9 - i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    buf[10] = '\0';
}
uint8_t hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return 0xFF; // Invalid
}
uint32_t hex_string_to_uint(const char *hex) {
  uint32_t result = 0;
  while (*hex) {
    char c = *hex++;
    if (c >= '0' && c <= '9') result = (result << 4) | (c - '0');
    else if (c >= 'A' && c <= 'F') result = (result << 4) | (c - 'A' + 10);
    else if (c >= 'a' && c <= 'f') result = (result << 4) | (c - 'a' + 10);
    else return 0; // Invalid
  }
  return result;
}
uint32_t hex_string_to_int(const char *str) {
  uint32_t result = 0;
  while (*str) {
    char c = *str++;
    result <<= 4;
    if (c >= '0' && c <= '9')
      result |= c - '0';
    else if (c >= 'A' && c <= 'F')
      result |= c - 'A' + 10;
    else if (c >= 'a' && c <= 'f')
      result |= c - 'a' + 10;
    else
      break;
  }
  return result;
}

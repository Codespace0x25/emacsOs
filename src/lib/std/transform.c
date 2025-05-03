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

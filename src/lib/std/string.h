#pragma once
#include <stdint.h>
#include <stddef.h>


size_t strlen(const char* str);

int strcmp(const char *a, const char *b);

char *strchr(const char *s, int c);

int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);

char* strtok_r(char* str, const char* delim,char** saveptr);
char* strtok(char* str, const char* delim);

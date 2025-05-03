#pragma once
#include <stddef.h>
#include <stdint.h>

void *memset(void *dest, int val, size_t len);

void *memcpy(void *dest, const void *src, size_t len);

int memcmp(const void *a, const void *b, size_t len);

void *memmove(void *dest, const void *src, size_t len);

int strcasecmp(const char* a, const char* b) ;

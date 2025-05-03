#include <stddef.h>
#include <stdint.h>
#include "../Screen.h"
#include "transform.h"

// Set `len` bytes starting from `dest` to value `val`
void *memset(void *dest, int val, size_t len) {
    unsigned char *ptr = dest;
    while (len--) {
        *ptr++ = (unsigned char)val;
    }
    return dest;
}

// Copy `len` bytes from `src` to `dest`
void *memcpy(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (len--) {
        *d++ = *s++;
	putS("read: ");
	char* cash = 0;
	int_to_string(len, cash);
	putS(cash);
	putS("\n");
    }
    return dest;
}

// Compare `len` bytes of `a` and `b`
int memcmp(const void *a, const void *b, size_t len) {
    const unsigned char *p1 = a;
    const unsigned char *p2 = b;
    for (size_t i = 0; i < len; i++) {
        if (p1[i] != p2[i])
            return p1[i] - p2[i];
    }
    return 0;
}

// Safer copy in case of overlapping memory regions
void *memmove(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    if (d == s) {
        return dest;
    }

    if (s < d && d < s + len) {
        // Copy backwards
        d += len;
        s += len;
        while (len--) {
            *(--d) = *(--s);
        }
    } else {
        // Copy forward
        while (len--) {
            *d++ = *s++;
        }
    }

    return dest;
}
int strcasecmp(const char* a, const char* b) {
    while (*a && *b) {
        char ca = *a;
        char cb = *b;

        // Convert both to uppercase
        if (ca >= 'a' && ca <= 'z') ca -= 32;
        if (cb >= 'a' && cb <= 'z') cb -= 32;

        if (ca != cb) return 1; // not equal
        a++;
        b++;
    }

    return (*a == '\0' && *b == '\0') ? 0 : 1;
}

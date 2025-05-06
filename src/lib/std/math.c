#include <stdint.h>
#include <stddef.h>
#include "Math.h"

#define MATH_EPSILON 1e-5f

float math_absf(float x) {
    return x < 0.0f ? -x : x;
}

int32_t math_absi(int32_t x) {
    return x < 0 ? -x : x;
}

float math_powf(float base, int32_t exp) {
    float result = 1.0f;
    int negative = 0;
    if (exp < 0) {
        exp = -exp;
        negative = 1;
    }
    for (int32_t i = 0; i < exp; i++) {
        result *= base;
    }
    return negative ? 1.0f / result : result;
}

float math_sqrtf(float x) {
    if (x < 0.0f) return -1.0f; // invalid input
    float guess = x * 0.5f;
    float prev;
    do {
        prev = guess;
        guess = 0.5f * (guess + x / guess);
    } while (math_absf(guess - prev) > MATH_EPSILON);
    return guess;
}

float math_sinf(float x) {
    float term = x;
    float result = x;
    for (size_t i = 1; i < 7; i++) {
        term *= -x * x / ((2 * i) * (2 * i + 1));
        result += term;
    }
    return result;
}

float math_cosf(float x) {
    float term = 1.0f;
    float result = 1.0f;
    for (size_t i = 1; i < 7; i++) {
        term *= -x * x / ((2 * i - 1) * (2 * i));
        result += term;
    }
    return result;
}

float math_tanf(float x) {
    float c = math_cosf(x);
    if (math_absf(c) < MATH_EPSILON) return 1e6f; // simulate "infinity"
    return math_sinf(x) / c;
}

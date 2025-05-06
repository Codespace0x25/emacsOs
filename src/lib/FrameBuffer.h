#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t* address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t  bpp;     // bits per pixel (must be 32)
} framebuffer_t;

extern framebuffer_t fb;

void fb_init(uint32_t multiboot_info);
void fb_clear(uint32_t color);
void fb_draw_pixel(int x, int y, uint32_t color);
void fb_fill_rect(int x, int y, int w, int h, uint32_t color);

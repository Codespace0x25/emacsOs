#include "FrameBuffer.h"

#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8

framebuffer_t fb;

typedef struct {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) mb2_tag;

typedef struct {
    uint32_t framebuffer_addr_low;
    uint32_t framebuffer_addr_high;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint8_t  reserved[2];
} __attribute__((packed)) mb2_tag_fb;

void fb_init(uint32_t mb_info) {
    mb2_tag* tag = (mb2_tag*)(mb_info + 8);

    while (tag->type != 0) {
        if (tag->type == MULTIBOOT2_TAG_TYPE_FRAMEBUFFER) {
            mb2_tag_fb* fb_tag = (mb2_tag_fb*)tag;

            if (fb_tag->framebuffer_bpp != 32) {
                // Unsupported BPP
                while (1);
            }

            fb.address = (uint8_t*)(uintptr_t)(
                ((uint64_t)fb_tag->framebuffer_addr_high << 32) |
                fb_tag->framebuffer_addr_low
            );
            fb.width  = fb_tag->framebuffer_width;
            fb.height = fb_tag->framebuffer_height;
            fb.pitch  = fb_tag->framebuffer_pitch;
            fb.bpp    = fb_tag->framebuffer_bpp;
            return;
        }

        tag = (mb2_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    // If we reach here, framebuffer was not found
    while (1);
}

void fb_clear(uint32_t color) {
    for (uint32_t y = 0; y < fb.height; ++y) {
        for (uint32_t x = 0; x < fb.width; ++x) {
            fb_draw_pixel(x, y, color);
        }
    }
}

void fb_draw_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= (int)fb.width || y < 0 || y >= (int)fb.height) return;

    uint32_t* pixel = (uint32_t*)(fb.address + y * fb.pitch + x * 4);
    *pixel = color;
}

void fb_fill_rect(int x, int y, int w, int h, uint32_t color) {
    for (int dy = 0; dy < h; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            fb_draw_pixel(x + dx, y + dy, color);
        }
    }
}

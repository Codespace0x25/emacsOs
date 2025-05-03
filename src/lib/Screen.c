#include <stdint.h>
#include "Screen.h"
#include "DataPort.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_PORT_COMMAND 0x3D4
#define VGA_PORT_DATA    0x3D5
#define VGA_ADDRESS 0xB8000

static volatile uint16_t* VGA = (volatile uint16_t*)VGA_ADDRESS;

static uint8_t screen_color = DEFAULT_COLOR;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void Screen_SetColor(uint8_t fg, uint8_t bg) {
    screen_color = VGA_COLOR(fg, bg);
}

void Screen_DefaltColor() {
    screen_color = DEFAULT_COLOR;
}

void Screen_SetCursorPos(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
    uint16_t pos = y * VGA_WIDTH + x;

    outb(VGA_PORT_COMMAND, 0x0F);
    outb(VGA_PORT_DATA, (uint8_t)(pos & 0xFF));

    outb(VGA_PORT_COMMAND, 0x0E);
    outb(VGA_PORT_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

void Screen_BackSpace(){
    cursor_x -= 1;
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(VGA_PORT_COMMAND, 0x0F);
    outb(VGA_PORT_DATA, (uint8_t)(pos & 0xFF));

    outb(VGA_PORT_COMMAND, 0x0E);
    outb(VGA_PORT_DATA, (uint8_t)((pos >> 8) & 0xFF));
    
}
void Screen_Relaod(){
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(VGA_PORT_COMMAND, 0x0F);
    outb(VGA_PORT_DATA, (uint8_t)(pos & 0xFF));

    outb(VGA_PORT_COMMAND, 0x0E);
    outb(VGA_PORT_DATA, (uint8_t)((pos >> 8) & 0xFF));
    
    putS(" ");
}

void Screen_Clear() {
    uint16_t blank = (' ' | (screen_color << 8));
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA[i] = blank;
    }
    Screen_SetCursorPos(0, 0);
}

static void Screen_ScrollIfNeeded() {
    if (cursor_y >= VGA_HEIGHT) {
        for (int y = 1; y < VGA_HEIGHT; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                VGA[(y - 1) * VGA_WIDTH + x] = VGA[y * VGA_WIDTH + x];
            }
        }
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ' ' | (screen_color << 8);
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void putS(const char* msg) {
    for (int i = 0; msg[i] != '\0'; ++i) {
        char c = msg[i];

        if (c == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else if (c == '\r') {
            cursor_x = 0;
        }  else {
            VGA[cursor_y * VGA_WIDTH + cursor_x] = (screen_color << 8) | c;
            cursor_x++;
            if (cursor_x >= VGA_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
        }

        Screen_ScrollIfNeeded();
    }

    Screen_SetCursorPos(cursor_x, cursor_y);
}
// Function to set the video mode to 0x13 (VESA 320x200 mode)
void set_graphics_mode() {
    __asm__ volatile (
        "mov $0x0013, %%ax\n"  // Set video mode 0x13
        "int $0x10\n"           // Call BIOS video interrupt
        : // No output
        : // No input
        : "%ax"                 // Clobber register
    );
}

// Function to set the video mode to 0x03 (text mode 80x25)
void set_text_mode() {
    __asm__ volatile (
        "mov $0x0003, %%ax\n"  // Set text mode 0x03 (80x25)
        "int $0x10\n"           // Call BIOS video interrupt
        : // No output
        : // No input
        : "%ax"                 // Clobber register
    );
}
void Screen_MoveCursorBack() {
    uint16_t pos = Screen_GetCursorPos();
    if (pos > 0) {
        pos--;
    }
    Screen_SetCursorPos(pos % VGA_WIDTH, pos / VGA_WIDTH);
}
uint16_t Screen_GetCursorPos() {
    outb(VGA_PORT_COMMAND, 0x0F); // Low byte of position
    uint8_t low = inb(VGA_PORT_DATA);
    outb(VGA_PORT_COMMAND, 0x0E); // High byte of position
    uint8_t high = inb(VGA_PORT_DATA);
    return (high << 8) | low;
}

void print_hex(uint32_t value) {
    char hex[9];
    for (int i = 0; i < 8; i++) {
        uint8_t byte = (value >> (28 - i * 4)) & 0xF; // Get each nibble (4 bits)
        if (byte < 10) {
            hex[i] = '0' + byte;  // Convert to '0'-'9'
        } else {
            hex[i] = 'A' + (byte - 10);  // Convert to 'A'-'F'
        }
    }
    hex[8] = '\0';  // Null terminate the string
    
    putS(hex);  // Print the hex string
}

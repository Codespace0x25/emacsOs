#pragma once
#include <stdint.h>

#define BLACK         0x0
#define BLUE          0x1
#define GREEN         0x2
#define CYAN          0x3
#define RED           0x4
#define MAGENTA       0x5
#define BROWN         0x6
#define LIGHT_GREY    0x7
#define DARK_GREY     0x8
#define LIGHT_BLUE    0x9
#define LIGHT_GREEN   0xA
#define LIGHT_CYAN    0xB
#define LIGHT_RED     0xC
#define LIGHT_MAGENTA 0xD
#define YELLOW        0xE
#define WHITE         0xF

#define VGA_COLOR(fg, bg) ((bg << 4) | (fg))

#define DEFAULT_COLOR VGA_COLOR(GREEN, BLACK)

void Screen_SetColor(uint8_t fg, uint8_t bg);
void Screen_DefaltColor();
void Screen_Clear();
void Screen_SetCursorPos(uint8_t x, uint8_t y);
void putS(const char* msg);
void set_graphics_mode();
void set_text_mode() ;
void Screen_Relaod();
void Screen_BackSpace();

void Screen_MoveCursorBack();
uint16_t Screen_GetCursorPos();

void disable_cursor();
void printf(const char *fmt, ...) ;

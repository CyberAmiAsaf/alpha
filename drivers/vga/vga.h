#ifndef ALPHA_VGA_H
#define ALPHA_VGA_H

#include <stddef.h>
#include <stdint.h>

#define VGA_MEMORY 0xb8000;

enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/* Cursor location on screen */
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

/* Get fg and bg color combination vga */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

/* Get color and char combination */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);

/* Clear vga screen */
void clear_screen();

/* Advance cursor by one character */
static inline void advance_cursor();

/* Print char */
void print_char_attr_loc(char c, enum vga_color fg, enum vga_color bg, int row, int col);
void print_char_attr(char c, enum vga_color fg, enum vga_color bg);
void print_char(char c);
#endif

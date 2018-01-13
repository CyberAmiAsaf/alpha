#ifndef ALPHA_VGA_H
#define ALPHA_VGA_H

#include "../types.h"

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

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* global cursor row/col */
extern u8 cursor_row;
extern u8 cursor_col;

/* Get fg and bg color combination vga */
u8 vga_entry_color(enum vga_color fg, enum vga_color bg);

/* Get color and char combination */
u16 vga_entry(unsigned char uc, u8 color);

/* Clear vga screen */
void clear_screen();

/* Print char */
void print_char_attr_loc(char c, enum vga_color fg, enum vga_color bg, int row, int col);
void print_char_attr(char c, enum vga_color fg, enum vga_color bg);
void print_char(char c);
void advance_cursor();

#endif

#ifndef ALPHA_VGA_H
#define ALPHA_VGA_H

#include <types.h>
#include <ports/ports.h>
#include <util/util.h>

#define VGA_MEMORY 0xb8000

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

/* cursor enable/disable/update */
void advance_cursor();
void backwards_cursor();
void disable_cursor();
void enable_cursor(u8 start, u8 end);
void update_cursor(u8 row, u8 col);

#endif

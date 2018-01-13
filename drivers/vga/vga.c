#include "vga.h"

/* Cursor location on screen */
u8 cursor_row = 0;
u8 cursor_col = 0;

/*
 * Get foreground and background color combination vga entry
 */
u8 vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
/*
 * Get unsigned character and color combination vga entry
 */
u16 vga_entry(unsigned char uc, u8 color) {
	return (u16) uc | (u16) color << 8;
}

/*
 * Clear screen
 */
void clear_screen() {
  register u16* vga_buffer = (u16*) VGA_MEMORY;
  const u8 color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  const u16 blank_space = vga_entry(' ', color);

  for (int y = 0; y < VGA_HEIGHT; ++y) {
    for (int x = 0; x < VGA_WIDTH; ++x) {
      const unsigned int index = y * VGA_WIDTH + x;
      vga_buffer[index] = blank_space;
    }
  }
}

/*
 * Print a character given character, foreground color, background color, row, column
 */
void print_char_attr_loc(char c, enum vga_color fg, enum vga_color bg, int row, int col) {
  u16* vga_buffer = (u16*) VGA_MEMORY;
  vga_buffer[row * VGA_WIDTH + col] = vga_entry(c, vga_entry_color(fg, bg));
}

/*
 * Print a character given character, foreground color, background color
 */
void print_char_attr(char c, enum vga_color fg, enum vga_color bg) {
  if (cursor_col == VGA_WIDTH || c == '\r' || c == '\n') {
    cursor_col = 0;
  }

  if (c == '\n') {
    ++cursor_row;
  }

  if (cursor_row >= VGA_HEIGHT) {
    /* TODO: scroll one row */
  }

  if (c != '\r' && c != '\n') {
    print_char_attr_loc(c, fg, bg, cursor_row, cursor_col);
    advance_cursor();
  }
}

/*
 * Print a character given character
 */
void print_char(char c) {
  print_char_attr(c, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

/* Advance cursor by one character */
void advance_cursor() {
  ++cursor_col;
  if (cursor_col == VGA_WIDTH) {
    ++cursor_row;

    if (cursor_row >= VGA_HEIGHT) {
      /* TODO: scroll one row */
    }
  }
}

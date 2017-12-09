#include "vga.h"

/*
 * Get foreground and background color combination vga entry
 */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
/*
 * Get unsigned character and color combination vga entry
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

/*
 * Clear screen
 */
void clear_screen() {
  register uint16_t* vga_buffer = (uint16_t*) VGA_MEMORY;
  const uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  const uint16_t blank_space = vga_entry(' ', color);

  for (size_t y = 0; y < VGA_HEIGHT; ++y) {
    for (size_t x = 0; x < VGA_WIDTH; ++x) {
      const size_t index = y * VGA_WIDTH + x;
      vga_buffer[index] = blank_space;
    }
  }
}

/*
 * Print a character given character, foreground color, background color, row, column
 */
void print_char_attr_loc(char c, enum vga_color fg, enum vga_color bg, int row, int col) {
  uint16_t* vga_buffer = (uint16_t*) VGA_MEMORY;
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
static inline void advance_cursor() {
  ++cursor_col;
  if (cursor_col == VGA_WIDTH) {
    ++cursor_row;

    if (cursor_row >= VGA_HEIGHT) {
      /* TODO: scroll one row */
    }
  }
}
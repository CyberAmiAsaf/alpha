#include "vga.h"

/* Cursor location on screen */
u8 cursor_row = 1;
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

  for (int y = 1; y < VGA_HEIGHT; ++y) {
    for (int x = 0; x < VGA_WIDTH; ++x) {
      const unsigned int index = y * VGA_WIDTH + x;
      vga_buffer[index] = blank_space;
    }
  }

  cursor_row = 1;
  cursor_col = 0;
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

  if (cursor_row > VGA_HEIGHT - 1) {
    void *dest = (void *) VGA_MEMORY + VGA_WIDTH * 2;
    void *src = (void *) VGA_MEMORY + VGA_WIDTH * 4;
    memcpy(dest, src, VGA_WIDTH * 2 * (VGA_HEIGHT - 2));

    for (int i = 0; i < VGA_WIDTH; i++) {
      print_char_attr_loc(' ', VGA_COLOR_WHITE, VGA_COLOR_BLACK, VGA_HEIGHT - 1, i);
    }

    cursor_row = VGA_HEIGHT - 1;
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

/*
 * Advance cursor by one character
 */
void advance_cursor() {
  ++cursor_col;
  if (cursor_col == VGA_WIDTH) {
    ++cursor_row;

    if (cursor_row >= VGA_HEIGHT) {
      cursor_row = VGA_HEIGHT - 1;
      cursor_col = 0;
    }
  }
}

/*
 * disable cursor
 */
void disable_cursor() {
  port_byte_out(0x3d4, 0x0a);
  port_byte_out(0x3d5, 0x20);
}

/*
 * enable cursor
 */
void enable_cursor(u8 start, u8 end) {
  port_byte_out(0x3d4, 0x0a);
  port_byte_out(0x3d5, (port_byte_in(0x3d5) & 0xc0) | start);

  port_byte_out(0x3d4, 0x0b);
  port_byte_out(0x3d5, (port_byte_in(0x3e0) & 0xe0) | end);
}

/*
 * update cursor position on sceren
 */
void update_cursor(u8 row, u8 col) {
  u16 pos = row * VGA_WIDTH + col;

  port_byte_out(0x3d4, 0x0f);
  port_byte_out(0x3d5, (u8) (pos & 0xff));
  port_byte_out(0x3d4, 0x0e);
  port_byte_out(0x3d5, (u8) ((pos >> 8) & 0xff));
}

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
 * Get unsigned character and color combination vga entry
 */
void clear_screen() {
  size_t row = 0;
  size_t column = 0;

  uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  uint16_t blank_space = vga_entry('S', color);

  uint16_t* vga_buffer = VGA_MEMORY;

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      vga_buffer[index] = blank_space;
    }
  }
}

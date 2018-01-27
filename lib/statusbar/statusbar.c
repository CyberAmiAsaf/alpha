#include "statusbar.h"

static char *current_program;
static char *os_name;

void init_statusbar(char *program_name) {
  current_program = program_name;
  os_name = "alpha os";
}

void update_statusbar() {
  u8 prev_row = cursor_row;
  u8 prev_col = cursor_col;

  cursor_row = 0;
  cursor_col = 0;

  int pname_len = strlen(current_program);
  int osname_len = strlen(os_name);
  int time_len = 8;

  enum vga_color fg = VGA_COLOR_BLACK;
  enum vga_color bg = VGA_COLOR_WHITE;

  puts_color(current_program, fg, bg);

  for (int i = 0; i < ((VGA_WIDTH / 2) - (osname_len / 2) - (pname_len)); i++) {
    print_char_attr(' ', fg, bg);
  }

  if (osname_len % 2 == 1) {
    print_char_attr(' ', fg, bg);
  }

  puts_color(os_name, fg, bg);

  for (int i = 0; i < ((VGA_WIDTH / 2) - (osname_len / 2) - (time_len)); i++) {
    print_char_attr(' ', fg, bg);
  }

  printf_color(fg, bg, "%2d:%2d:%2d", hw_time.hour, hw_time.min, hw_time.sec);

  cursor_row = prev_row;
  cursor_col = prev_col;
}

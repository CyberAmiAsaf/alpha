#include "shell.h"

void print_ps1() {
  printf(PS1);
  enable_cursor(0, 255);
  update_cursor(cursor_row, cursor_col);
}

void reboot() {
  u8 good = 0x02;
  while (good & 0x02) {
    good = port_byte_in(0x64);
  }

  port_byte_out(0x64, 0xfe);
  __asm__("hlt");
}

int execute_command(char *command_line) {
  char arguments[16][32];
  int arg = 0;
  int arg_ch = 0;

  disable_cursor();

  // parse arguments
  char *str_ptr = command_line;

  // null args
  memset(arguments, 0, 16 * 32);

  // parse loop
  while (*str_ptr) {
    while (*str_ptr && isspace(*str_ptr)) {
      ++str_ptr;
    }

    while (*str_ptr && !isspace(*str_ptr)) {
      arguments[arg][arg_ch] = *str_ptr;

      ++str_ptr;
      ++arg_ch;
    }

    ++arg;
    arg_ch = 0;
  }

  if (strlen(arguments[0]) == 0) {
  } else if (strcmp(arguments[0], "clear")) {
    clear_screen();
  } else {
    printf("'%s' is not a recognized command (%d args inputted)\n", arguments[0], arg);
  }

  printf(PS1);
  enable_cursor(0, 255);
  update_cursor(cursor_row, cursor_col);
  return 0;
}

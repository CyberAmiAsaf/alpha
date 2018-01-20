#include "util.h"

void puts(const char *str) {
  while(*str) {
    print_char(*str++);
  }
}

void puts_color(const char *str, enum vga_color bg, enum vga_color fg) {
  while(*str) {
    print_char_attr(*str++, fg, bg);
  }
}

int vprintf(const char *fmt, va_list arg) {
  register char ch;
  int length = 0;

  int tmp_int;
  char tmp_chr;
  char *tmp_str;
  char tmp_buf[16];

  while (ch = *fmt++) {
    if (ch == '%') {
      switch (ch = *fmt++) {
        case '%':
          print_char('%');
          length++;
          break;

        case 'c':
          tmp_chr = va_arg(arg, int);
          print_char(tmp_chr);
          break;

        case 's':
          tmp_str = va_arg(arg, char*);
          puts(tmp_str);
          break;

        case 'd':
          tmp_int = va_arg(arg, int);
          itoa(tmp_int, tmp_buf);
          puts(tmp_buf);
          break;

        case '\0':
          return length;

        default:
          print_char('?');
          break;
      }
    } else {
      print_char(ch);
      ++length;
    }
  }

  return 0;
}

int printf(const char *fmt, ...) {
  va_list arg;
  int length;

  va_start(arg, fmt);
  length = vprintf(fmt, arg);
  va_end(arg);

  return length;
}

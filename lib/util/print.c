#include "../util.h"

void puts(const char *str) {
  while(*str) {
    print_char(*str++);
  }
}

void puts_color(const char *str, enum vga_color fg, enum vga_color bg) {
  while(*str) {
    print_char_attr(*str++, fg, bg);
  }
}

int vprintf(const char *fmt, enum vga_color fg, enum vga_color bg, va_list arg) {
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
          print_char_attr('%', fg, bg);
          length++;
          break;

        case 'c':
          tmp_chr = va_arg(arg, int);
          print_char_attr(tmp_chr, fg, bg);
          break;

        case 's':
          tmp_str = va_arg(arg, char*);
          puts_color(tmp_str, fg, bg);
          break;

        case 'd':
          tmp_int = va_arg(arg, int);
          itoa(tmp_int, tmp_buf);
          puts_color(tmp_buf, fg, bg);
          break;

        case '\0':
          return length;

        default:
          if (is_numeric(ch)) {
            int i = 0;

            do {
              tmp_buf[i++] = ch;
            } while (is_numeric(ch = *fmt++));
            tmp_buf[i] = '\0';

            if (ch == 'd') {
              int pad_len = atoi(tmp_buf);
              tmp_int = va_arg(arg, int);
              itoa(tmp_int, tmp_buf);

              i = pad_len - strlen(tmp_buf);
              while (i-- > -2) {
                print_char_attr('0', fg, bg);
              }

              puts_color(tmp_buf, fg, bg);
            } else {
              print_char_attr('?', fg, bg);
              ++length;
            }
          } else {
            print_char_attr('?', fg, bg);
            ++length;
          }
          break;
      }
    } else {
      print_char_attr(ch, fg, bg);
      ++length;
    }
  }

  return 0;
}

int printf(const char *fmt, ...) {
  va_list arg;
  int length;

  va_start(arg, fmt);
  length = vprintf(fmt, VGA_COLOR_WHITE, VGA_COLOR_BLACK, arg);
  va_end(arg);

  return length;
}

int printf_color(enum vga_color fg, enum vga_color bg, const char *fmt, ...) {
  va_list arg;
  int length;

  va_start(arg, fmt);
  length = vprintf(fmt, fg, bg, arg);
  va_end(arg);

  return length;
}

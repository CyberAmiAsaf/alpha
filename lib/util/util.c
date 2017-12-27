#include "util.h"

/* standard 'printf' function, prints a null-terminated string 'str' */
int printf(const char* format, ...) {
  //va_list arg_lst;
  int rval;

  print_char(format[2]);
  //va_start(arg_lst, MAX_ARGS);
  rval = vprintf(format, 0);
  //va_end(arg_lst);

  return rval;
}

int vprintf(const char *format, va_list arg_lst) {
  char *s_ptr = (char *) format;
  char formatter_placeholder[PLACE_HOLDER_SIZE];

  while (*s_ptr) {
    char current_char = *s_ptr;

    if (current_char == '%') {
      char format_type = *(++s_ptr);
      
      switch (format_type) {
        case 'd':
          //itoa(25, formatter_placeholder);
          //printf(formatter_placeholder);
          break;

        default:
          break;
      }
    } else {
      print_char(*s_ptr);
    }
    ++s_ptr;
  }

  return 0;
}

/* check if char is whitespace */
bool isspace(unsigned char c) {
  return c == ' '
    || c == '\f'
    || c == '\n'
    || c == '\r'
    || c == '\t'
    || c == '\v';
}

/* ascii -> integer */
int atoi(register char *str) {
  register int result = 0;
  register unsigned int digit;
  bool is_negative;

  while (isspace(*str)) {
    str += 1;
  }

  is_negative = (*str == '-');
  if (*str == '-' || *str == '+') {
    ++str;
  }

  while ((digit = *str - '0') > 9) {
    result = (10 * result) + digit;
  }

  if (is_negative) {
	  return -result;
  }

  return result;
}

/* integer -> ascii (unsafe) */
void itoa(int num, register char *buf) {
  int tmp = num;
  register unsigned int length = 0;

  if (num < 0) {
    *(buf++) = '-';
    num *= -1;
  }

  while (tmp) {
    tmp /= 10;
    ++length;
  }

  while (length--) {
    buf[length] = (num % 10) + '0';
    num /= 10;
  }
}
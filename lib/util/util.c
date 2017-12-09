#include "util.h"

/* standard 'printf' function, prints a null-terminated string 'str' */
void printf(char *str) {
  while (*str) {
    print_char(*str);
    ++str;
  }
}
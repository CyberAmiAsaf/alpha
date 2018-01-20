#include "util.h"

int split_string(char *str, char split_by, char **args) {
  int splitted = 0;

  return splitted;
}

int strlen(register char *str) {
  register int size = 0;

  while (*str++ != '\0') {
    size++;
  }

  return size;
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
void itoa(register int num, register char *buf) {
  int tmp = num;
  unsigned int length = 0;

  if (num < 0) {
    *(buf++) = '-';
    num *= -1;
  }

  while (tmp) {
    tmp /= 10;
    ++length;
  }

  buf[length] = '\0';
  while (length--) {
    buf[length] = (num % 10) + '0';
    num /= 10;
  }
}

void to_uppercase(char *str) {
  char c;
  while (c = *str) {
    if (c > 'a' && c < 'z') {
      *str = c - 0x20;
    }

    str++;
  }
}

void to_lowercase(char *str) {
  char c;
  while (c = *str) {
    if (c > 'A' && c < 'Z') {
      *str = c + 0x20;
    }

    str++;
  }
}

bool strcmp(char *s1, char *s2) {
  while (*s1 && *s1++ == *s2++);
  return *s1 == *s2;
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

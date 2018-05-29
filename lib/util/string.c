#include "../util.h"

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
char *itoa(int num, register char *buf) {
  int tmp = num;
  unsigned int length = 0;

  if (num == 0) {
    buf[0] = '0';
    buf[1] = '\0';
    return buf;
  }

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

  return buf;
}

char *pad_string(char *buf, char padc, int n) {
  register int buflen = strlen(buf);
  if (n > buflen) {
    memcpy(buf + (n - buflen), buf, buflen);
    memset(buf, (int)padc, n - buflen);
  }

  return buf;
}

char *to_uppercase(char *str) {
  char c;
  while (c = *str) {
    if (c > 'a' && c < 'z') {
      *str = c - 0x20;
    }

    str++;
  }

  return str;
}

char *to_lowercase(char *str) {
  char c;
  while (c = *str) {
    if (c > 'A' && c < 'Z') {
      *str = c + 0x20;
    }

    str++;
  }

  return str;
}

int strcmp(char *s1, char *s2) {
  while (*s1 && *s1++ == *s2++);
  return *s2 - *s1;
}

void *strcpy(const char *src, char *dest) {
  int i = 0;
  while (*src) {
    dest[i++] = *src++;
  }

  return dest;
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

// is alpha numeric
bool is_alphanumeric(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

// is alpha
bool is_alphabetic(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// is numeric
bool is_numeric(char c) {
  return c >= '0' && c <= '9';
}

#ifndef ALPHA_UTIL_H
#define ALPHA_UTIL_H

#include <stdarg.h>
#include "../../drivers/types.h"
#include "../../drivers/vga/vga.h"

// suppress unsused parameter warning
#define UNUSED(x) (void)(x)

// prints am unformatted string
void puts(const char *str);
void puts_color(const char *str, enum vga_color bg, enum vga_color fg);

// standard 'printf' function, prints a formatted null-terminated string
int vprintf(const char* format, va_list arg);
int printf(const char* format, ...);

// set memory value for len bytes
void *memset(void *ptr, int value, unsigned int len);
void *cpy(void *dest, void *src, unsigned int len);

// string - integer conversion
void itoa(int num, register char *buf);
int atoi(register char *str);
int strlen(register char *str);

// upper/lower case string
void to_uppercase(char *str);
void to_lowercase(char *str);

// strings
bool strcmp(char *s1, char *s2);

// isspace
bool isspace(unsigned char c);

#endif

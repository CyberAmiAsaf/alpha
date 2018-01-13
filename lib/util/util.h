#ifndef ALPHA_UTIL_H
#define ALPHA_UTIL_H

#include <stdarg.h>
#include "../../drivers/types.h"
#include "../../drivers/vga/vga.h"

// suppress unsused parameter warning
#define UNUSED(x) (void)(x)

// prints am unformatted string
void puts(const char *str);

// standard 'printf' function, prints a formatted null-terminated string
int vprintf(const char* format, va_list arg);
int printf(const char* format, ...);

// string - integer conversion
void itoa(int num, register char *buf);
int atoi(register char *str);

// upper/lower case string
void to_uppercase(char *str);
void to_lowercase(char *str);

// strings
bool strcmp(char *s1, char *s2);

// isspace
bool isspace(unsigned char c);

#endif

#ifndef ALPHA_UTIL_H
#define ALPHA_UTIL_H

#include <stdarg.h>
#include "../../drivers/vga/vga.h"

// suppress unsused parameter warning
#define UNUSED(x) (void)(x)

// define boolean
typedef int bool;
#define true 1
#define false 0

// prints am unformatted string
void puts(const char *str);

// standard 'printf' function, prints a formatted null-terminated string
int vprintf(const char* format, va_list arg);
int printf(const char* format, ...);

// string - integer conversion
void itoa(int num, register char *buf);
int atoi(register char *str);

// isspace
bool isspace(unsigned char c);

#endif

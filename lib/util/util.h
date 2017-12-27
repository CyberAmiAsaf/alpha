#ifndef ALPHA_UTIL_H
#define ALPHA_UTIL_H

#include <stdarg.h>
#include "../../drivers/vga/vga.h"

#define MAX_ARGS 256
#define PLACE_HOLDER_SIZE 128

/* define boolean */
typedef int bool;
#define true 1
#define false 0

/* standard 'printf' function, prints a null-terminated string */
int printf(const char* format, ...);
int vprintf(const char *format, va_list argptr);

/* string - integer conversion */
void itoa(int num, register char *buf);
int atoi(register char *str);

/* isspace */
//inline bool isspace(unsigned char c);

#endif
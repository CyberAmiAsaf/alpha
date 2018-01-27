#ifndef ALPHA_UTIL_H
#define ALPHA_UTIL_H

#include <stdarg.h>
#include <types.h>
#include <vga/vga.h>

// suppress unsused parameter warning
#define UNUSED(x) (void)(x)

// prints am unformatted string
void puts(const char *str);
void puts_color(const char *str, enum vga_color bg, enum vga_color fg);

// standard 'printf' function, prints a formatted null-terminated string
int vprintf(const char* format, enum vga_color fg, enum vga_color bg, va_list arg);
int printf(const char* format, ...);
int printf_color(enum vga_color fg, enum vga_color bg, const char* format, ...);

// set memory value for len bytes
void *memset(void *ptr, int value, unsigned int len);
void *memcpy(void *dest, void *src, unsigned int len);

// string - integer conversion
char *itoa(int num, register char *buf);
int atoi(register char *str);
int strlen(register char *str);

// upper/lower case string
char *to_uppercase(char *str);
char *to_lowercase(char *str);

// pading
char *pad_string(char *str, char padc, int n);

// strings
bool strcmp(char *s1, char *s2);

// isspace
bool isspace(unsigned char c);

// is alphanumeric
bool is_alphanumeric(char c);
bool is_alphabetic(char c);
bool is_numeric(char c);

#endif

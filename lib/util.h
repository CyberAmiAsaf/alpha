#ifndef ALPHA_UTIL_H
#define ALPHA_UTIL_H

#include <stdarg.h>
#include <types.h>
#include <vga.h>

// suppress unsused parameter warning
#define UNUSED(x) (void)(x)

// memory
#define BASE_HEAP (0x2000)
#define pages_size (1<<30)
#define len_curve 16

unsigned long heap_pointer;

// prints am unformatted string
void puts(const char *str);
void puts_color(const char *str, enum vga_color bg, enum vga_color fg);

// standard 'printf' function, prints a formatted null-terminated string
int vprintf(const char* format, enum vga_color fg, enum vga_color bg, va_list arg);
int printf(const char* format, ...);
int printf_color(enum vga_color fg, enum vga_color bg, const char* format, ...);

// set memory value for len bytes
typedef struct {
	char used;
	unsigned int data_size;
	unsigned int addr;
} __attribute__((packed)) Header;

typedef struct {
	Header* headers;
	int len;
}__attribute__((packed)) Block;

void *memset(void *ptr, int value, unsigned int len);
void *memcpy(void *dest, void *src, unsigned int len);

void init_pages(unsigned int size);
char* malloc(unsigned int n_bytes);
void free(unsigned int ptr);
char find_category(int n);


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
int strcmp(char *s1, char *s2);
void *strcpy(const char *src, char *dest);

// isspace
bool isspace(unsigned char c);

// is alphanumeric
bool is_alphanumeric(char c);
bool is_alphabetic(char c);
bool is_numeric(char c);

// integers
int get_num_digits(int i);

#endif

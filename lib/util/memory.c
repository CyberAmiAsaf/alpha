#include "util.h"

void *memset(void *ptr, int value, unsigned int len) {
  while (len--) {
    ((u8 *)ptr)[len] = value;
  }

  return ptr;
}

void *memcpy(void *dest, void *src, unsigned int len) {
  while (len--) {
    ((u8 *)dest)[len] = ((u8 *)src)[len];
  }

  return dest;
}
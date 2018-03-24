#include "../util.h"

void *memset(void *ptr, int value, unsigned int len) {
  while (len--) {
    ((u8 *)ptr)[len] = value;
  }

  return ptr;
}

void *memcpy(void *dest, void *src, unsigned int len) {
  unsigned int i = 0;

  while (i != len) {
    ((u8 *)dest)[i] = ((u8 *)src)[i];
    ++i;
  }

  return dest;
}

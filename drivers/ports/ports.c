#include "../ports.h"

u8 port_byte_in (u16 port) {
  unsigned char result;
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void port_byte_out (u16 port, u8 data) {
  __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

u16 port_word_in (u16 port) {
  unsigned short result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void port_word_out (u16 port, u16 data) {
  __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

void port_sl_in(u16 port, void* buf, unsigned int nr) {
	__asm__ ("cld;rep;insl\n\t" :: "d"(port), "D"(buf), "c"(nr));
}

void port_sl_out(u16 port, void* buf, unsigned int nr) {
	__asm__ ("cld;rep;outsl\n\t" :: "d"(port), "S" (buf), "c" (nr));
}

void disable_ints () {
  __asm__("cli");
}

void enable_ints () {
  __asm__("sti");
}

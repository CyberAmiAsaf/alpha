#ifndef ALPHA_PORTS_H
#define ALPHA_PORTS_H

#include <types.h>

// Byte in/out
u8 port_byte_in (u16 port);
void port_byte_out (u16 port, u8 data);

// Word in/out
u16 port_word_in (u16 port);
void port_word_out (u16 port, u16 data);

// sl
void port_sl_in(u16 port, void* buf, unsigned int nr);
void port_sl_out(u16 port, void* buf, unsigned int nr);

// Disable interrupts
void disable_ints();

// Enable interrupts
void enable_ints();
#endif

#ifndef ALPHA_PORTS_H
#define ALPHA_PORTS_H

#include "../types.h"

// Byte in/out
u8 port_byte_in (u16 port);
void port_byte_out (u16 port, u8 data);

// Word in/out
u16 port_word_in (u16 port);
void port_word_out (u16 port, u16 data);
#endif

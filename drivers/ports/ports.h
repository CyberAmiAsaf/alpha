#ifndef ALPHA_PORTS_H
#define ALPHA_PORTS_H

#include <stddef.h>
#include <stdint.h>

// Byte in/out
uint8_t port_byte_in (uint16_t port);
void port_byte_out (uint16_t port, uint8_t data);

// Word in/out
uint16_t port_word_in (uint16_t port);
void port_word_out (uint16_t port, uint16_t data);
#endif
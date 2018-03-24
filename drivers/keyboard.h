#ifndef ALPHA_KEYBOARD_H
#define ALPHA_KEYBOARD_H

#include <isr.h>
#include <util.h>
#include <shell.h>
#include <ports.h>

// initialize the keyboard callback
void init_keyboard();
bool should_use_uppercase();

#endif

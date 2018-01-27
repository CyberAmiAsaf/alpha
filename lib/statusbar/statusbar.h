#ifndef ALPHA_STATUSBAR_H
#define ALPHA_STATUSBAR_H

#include <types.h>
#include <util/util.h>
#include <vga/vga.h>
#include <timer/timer.h>

void init_statusbar(char *program_name);
void update_statusbar();

#endif

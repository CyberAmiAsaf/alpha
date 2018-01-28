#ifndef ALPHA_SHELL_H
#define ALPHA_SHELL_H

#include <util/util.h>

#define PS1 "ALPHA$ "

void print_ps1();
int execute_command(char *str);

#endif

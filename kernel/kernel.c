#include <stddef.h>
#include <stdint.h>

#include "../lib/util/util.h"
#include "../drivers/vga/vga.h"

#define DEBUG 1

int start_kernel() {
  clear_screen();

  printf("hello: ", 25);

  return 0;
}

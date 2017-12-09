#include "../drivers/vga/vga.h"
#include "../lib/util/util.h"

#define DEBUG 1

int start_kernel() {
  clear_screen();

  printf("Successfuly transferred from 16-bit real-mode to 32-bit protected-mode\n");
  printf("Control was transferred by the BIOS to AlphaOS\n");
  printf("AlphaOS successfuly initiated\n");

  return 0;
}

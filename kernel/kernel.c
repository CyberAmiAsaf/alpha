#include <stddef.h>
#include <stdint.h>

#include "../cpu/isr/isr.h"

#include "../drivers/types.h"
#include "../drivers/vga/vga.h"

#include "../lib/util/util.h"

#define DEBUG 1

int start_kernel() {
  clear_screen();

  isr_install();
  irq_install();

  while(true);

  return 0;
}

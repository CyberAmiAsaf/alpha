#include "kernel.h"

#include "../cpu/isr/isr.h"

#include "../drivers/types.h"
#include "../drivers/vga/vga.h"

#include "../lib/util/util.h"
#include "../lib/logger/logger.h"

#define DEBUG 1

int start_kernel() {
  clear_screen();

  log("info", "booting up..");
  log("info", "installing isr...");
  log("info", "installing irq...");

  isr_install();
  irq_install();

  log("ok", "isr is up and running");
  log("info", "alpha os successfully initiated");

  while(true);

  return 0;
}

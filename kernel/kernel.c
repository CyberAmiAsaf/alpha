#include "kernel.h"

#include "../cpu/isr/isr.h"

#include "../drivers/types.h"
#include "../drivers/vga/vga.h"
#include "../drivers/timer/timer.h"

#include "../lib/util/util.h"
#include "../lib/logger/logger.h"

#define DEBUG 1

int start_kernel() {
  clear_screen();

  log("info", "booting up..");

  log("info", "installing isr...");
  isr_install();
  log("ok", "isr is up and running"); 
 
  log("info", "installing irq...");
  log("info", "installing timer irq...");
  log("info", "installing keyboard irq...");
  irq_install();
  log("ok", "irqs successfuly installed"); 

  log("info", "alpha os successfully initiated");
  log("fail", "an error occurred while attempting to do nothing");

  while(true) {

  }

  return 0;
}

#include "kernel.h"

#define DEBUG 1

int start_kernel() {
  clear_screen();

  log("info", "booting up..");

  log("info", "initiating cmos (rtc)...");
  int cmos_res = cmos_init();
  if (cmos_res == 0) {
    log("ok", "cmos (rtc) successfully initiated");
  } else {
    log("fail", "failed while initiating cmos (rtc)");
  }

  log("info", "installing isr...");
  isr_install();
  log("ok", "isr is up and running");

  log("info", "installing irq...");
  log("info", "installing timer irq...");
  log("info", "installing keyboard irq...");
  irq_install();
  log("ok", "irqs successfuly installed"); 

  log("info", "initiating statusbar...");
  init_statusbar("shell");
  update_statusbar();

  log("info", "alpha os successfully initiated");

  print_ps1();

  while(true) {
  }

  return 0;
}

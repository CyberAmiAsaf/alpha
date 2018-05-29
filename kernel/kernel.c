#include "kernel.h"

#define DEBUG 1

int start_kernel() {
  init_pages(pages_size);
  clear_screen();
  log("info", "booting up..");

  log("info", "initiating cmos (rtc)...");
  int cmos_res = cmos_init();
  if (cmos_res == 0) {
    log("ok", "cmos (rtc) successfully initiated");
  } else {
    log("fail", "initiating cmos (rtc)");
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

  log("info", "verifiying hd partition table...");

  bool hd_res = verify_alpha_hd();
  if (hd_res == true) {
    log("ok", "hd integrity is well");
  } else {
    log("warn", "hd integrity could not be found");
    log("info", "applying setup on hd...");
    init_alpha_hd();
    log("ok", "hd integrity is well now");
  }

  bool fs_res = verify_alpha_fs();
  if (fs_res == true) {
    log("ok", "fs integrity is well");
  } else {
    log("warn", "fs is curropted");
    log("info", "re-creating root dir");
    init_alpha_fs();
    log("ok", "fs integrity is well now");
  }

  can_type = true;
  log("info", "alpha os successfully initiated");

  print_ps1();

  while(true) {}

  return 0;
}

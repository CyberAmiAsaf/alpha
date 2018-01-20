#ifndef ALPHA_TIMER_H
#define ALPHA_TIMER_H

#include "../types.h"

#include "../ports/ports.h"
#include "../../cpu/isr/isr.h"
#include "../../lib/util/util.h"

#define INPUT_TIMER_HZ 1193180
#define timer_base (0x100000)
#define TICK_TIME 1000

struct TimerBuffer {
  int timer_ticks;
};

void init_timer();
void timer_phase(int hz);
static void timer_callback(registers_t regs);
void timer_wait(int ticks);

#endif
#include "timer.h"

void timer_phase(int hz) {
  // divisor
  int divisor = INPUT_TIMER_HZ / hz;

  port_byte_out(0x43, 0x36);

  // lsb
  port_byte_out(0x40, divisor & 0xFF);
  // msb
  port_byte_out(0x40, divisor >> 8);
}

static void timer_callback(registers_t regs) {
  // inc tick count
  struct TimerBuffer * timerBuffer = (struct TimerBuffer *) timer_base;
  timerBuffer->timer_ticks++;
}

// install timer_callback in irq0
void init_timer() {
	struct TimerBuffer * timerBuffer = (struct TimerBuffer *)timer_base;
	timerBuffer->timer_ticks = 0;
	
  // tick time
  timer_phase(TICK_TIME);
  
  register_interrupt_handler(IRQ0, (isr_t)timer_callback);
}

// loop until number of milliseconds has been reached
void timer_wait(int ticks) {
  struct TimerBuffer * timerBuffer = (struct TimerBuffer *)timer_base;
  unsigned long eticks;

  eticks = timerBuffer->timer_ticks + ticks;
  while(timerBuffer->timer_ticks < eticks);
}
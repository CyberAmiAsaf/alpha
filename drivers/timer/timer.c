#include "../timer.h"

struct time_t hw_time;

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

  struct time_t time_to_add = {.day = 0, .hour = 0, .min = 0, .sec = 0, .ms = 1};
  add_time(&hw_time, &time_to_add);

  if (timerBuffer->timer_ticks % 1000 == 0) {
    update_statusbar();
  }
}

/*
 * Retrieves correct time from CMOS.
 */
inline static struct time_t *update_clock_hw(struct time_t *t)
{
    int time = rtc_get_time();

    t->day = 0;
    t->hour = RTC_TO_HOUR(time);
    t->min = RTC_TO_MIN(time);
    t->sec = RTC_TO_SEC(time);
    t->ms = 0;

    return t;
}

// install timer_callback in irq0
void init_timer() {
  struct TimerBuffer * timerBuffer = (struct TimerBuffer *)timer_base;
  timerBuffer->timer_ticks = 0;

  // tick time
  timer_phase(TICK_TIME);

  // init
  update_clock_hw(&hw_time);

  register_interrupt_handler(IRQ0, (isr_t)timer_callback);
}

// loop until number of milliseconds has been reached
void timer_wait(int ticks) {
  struct TimerBuffer * timerBuffer = (struct TimerBuffer *)timer_base;
  unsigned long eticks;

  eticks = timerBuffer->timer_ticks + ticks;
  while(timerBuffer->timer_ticks < eticks);
}

// add time from t2 to t1, t1 and t2 are both supposed to be valid
void add_time(struct time_t *t1, struct time_t *t2) {
  t1->ms += t2->ms;
  t1->sec += t2->sec;
  t1->min += t2->min;
  t1->hour += t2->hour;
  t1->day += t2->day;

  if (t1->ms > 999) {
    t1->ms = t1->ms - 1000;
    t1->sec += 1;
  }

  if (t1->sec > 59) {
    t1->sec = t1->sec - 60;
    t1->min += 1;
  }

  if (t1->min > 59) {
    t1->min = t1->min - 60;
    t1->hour += 1;
  }

  if (t1->hour > 23) {
    t1->hour = t1->hour - 24;
    t1->day += 1;
  }
}


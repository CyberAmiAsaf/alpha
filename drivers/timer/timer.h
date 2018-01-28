#ifndef ALPHA_TIMER_H
#define ALPHA_TIMER_H

#include <types.h>

#include <rtc/rtc.h>
#include <ports/ports.h>
#include <isr/isr.h>
#include <util/util.h>
#include <statusbar/statusbar.h>

#define INPUT_TIMER_HZ 1193180
#define timer_base (0x100000)
#define TICK_TIME 1000 //hz

struct TimerBuffer {
  int timer_ticks;
};

extern struct time_t hw_time;
typedef unsigned long milis_t;

#define RTC_TO_SEC(t)   \
    ((t) & 0xFF)
#define RTC_TO_MIN(t)   \
    ((t) >> 8)
#define RTC_TO_HOUR(t)  \
    ((t) >> 16)

void init_timer();
void timer_phase(int hz);
static void timer_callback(registers_t regs);
void timer_wait(int ticks);
void add_time(struct time_t *t1, struct time_t *t2);
inline static struct time_t *update_clock_hw(struct time_t *t);

#endif

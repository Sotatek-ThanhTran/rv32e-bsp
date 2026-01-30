/*
 * BSP utility functions: delay and timing (mtime-based)
 * Assumes mtime runs at CLK_HZ.
 */

#include "util.h"
#include "hw_registers.h"

#define TICKS_PER_MS  (CLK_HZ / 1000)
#define TICKS_PER_US  (CLK_HZ / 1000000)

static volatile unsigned int *const mtime_lo = (volatile unsigned int *)TIMER_MTIME_LO;
static volatile unsigned int *const mtime_hi = (volatile unsigned int *)TIMER_MTIME_HI;

static unsigned long long get_mtime(void) {
  unsigned int hi, lo;
  do {
    hi = *mtime_hi;
    lo = *mtime_lo;
  } while (hi != *mtime_hi);
  return ((unsigned long long)hi << 32) | lo;
}

void delay_ms(int ms) {
  if (ms <= 0)
    return;
  unsigned long long until = get_mtime() + (unsigned long long)ms * TICKS_PER_MS;
  while (get_mtime() < until)
    ;
}

void delay_us(int us) {
  if (us <= 0)
    return;
  unsigned long long until = get_mtime() + (unsigned long long)us * TICKS_PER_US;
  while (get_mtime() < until)
    ;
}

unsigned long get_tick_ms(void) {
  return (unsigned long)(get_mtime() / TICKS_PER_MS);
}

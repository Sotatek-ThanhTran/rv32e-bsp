/*
 * BSP utility functions: delay, timing, and other common helpers
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * @brief Delay for at least the given number of milliseconds
 * @param ms Number of milliseconds to wait
 */
void delay_ms(int ms);

/**
 * @brief Delay for at least the given number of microseconds
 * @param us Number of microseconds to wait
 */
void delay_us(int us);

/**
 * @brief Get milliseconds since boot (from mtime)
 * @return Millisecond count (may wrap after ~49 days)
 */
unsigned long get_tick_ms(void);

#endif /* UTIL_H */

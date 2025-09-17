#ifndef SPUDKIT_TIMER_H
#define SPUDKIT_TIMER_H

#include <stdint.h>

// Timer modes
#define TIMER_MODE_ONESHOT 0
#define TIMER_MODE_PERIODIC 1

// Timer initialization
void timer_init(void);

// Basic timer functions
void timer_start(uint32_t ticks);
void timer_stop(void);
uint32_t timer_get_count(void);
uint8_t timer_is_expired(void);

// Delay functions
void delay_ms(uint32_t milliseconds);
void delay_us(uint32_t microseconds);

// Timer callbacks (if interrupts are enabled)
typedef void (*timer_callback_t)(void);
void timer_set_callback(timer_callback_t callback);

#endif // SPUDKIT_TIMER_H
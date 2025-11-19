#include "timer.h"
#include "spudkit.h"

static timer_callback_t timer_cb = 0;

void timer_init(void) {
    // TODO: Initialize timer controller
}

void timer_start(uint32_t ticks) {
    // TODO: Start timer with specified tick count
    volatile uint32_t* timer_ctrl = (volatile uint32_t*)(SPUD_TIMER_BASE + 0x00);
    volatile uint32_t* timer_load = (volatile uint32_t*)(SPUD_TIMER_BASE + 0x04);

    *timer_load = ticks;
    *timer_ctrl = 1; // Enable timer
}

void timer_stop(void) {
    // TODO: Stop timer
    volatile uint32_t* timer_ctrl = (volatile uint32_t*)(SPUD_TIMER_BASE + 0x00);
    *timer_ctrl = 0; // Disable timer
}

uint32_t timer_get_count(void) {
    // TODO: Get current timer count
    volatile uint32_t* timer_count = (volatile uint32_t*)(SPUD_TIMER_BASE + 0x08);
    return *timer_count;
}

uint8_t timer_is_expired(void) {
    // TODO: Check if timer has expired
    volatile uint32_t* timer_status = (volatile uint32_t*)(SPUD_TIMER_BASE + 0x0C);
    return (*timer_status & 1) ? 1 : 0;
}

void delay_ms(uint32_t milliseconds) {
    // Busy-wait delay calibrated for 25MHz CPU clock
    // Loop overhead: ~5 cycles per iteration (increment, compare, branch, nop)
    // So we need 25000 / 5 = 5000 iterations per millisecond
    volatile uint32_t iterations = milliseconds * 5000;
    for(volatile uint32_t i = 0; i < iterations; i++) {
        asm volatile("nop");
    }
}

void delay_us(uint32_t microseconds) {
    // Simple busy-wait delay (CPU clock is 25MHz, not 100MHz)
    volatile uint32_t cycles = microseconds * 25; // 25 cycles per us at 25MHz
    for(volatile uint32_t i = 0; i < cycles; i++) {
        asm volatile("nop");
    }
}

void timer_set_callback(timer_callback_t callback) {
    timer_cb = callback;
}

// Timer interrupt handler (would be called by IRQ system)
void __attribute__((weak)) timer_irq_handler(void) {
    if (timer_cb) {
        timer_cb();
    }
}
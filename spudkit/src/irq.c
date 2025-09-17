#include "irq.h"
#include "spudkit.h"

static irq_handler_t irq_handlers[8] = {0}; // Support for 8 IRQs

void irq_init(void) {
    // TODO: Initialize interrupt controller
    // Clear all pending interrupts
    volatile uint32_t* irq_clear = (volatile uint32_t*)(SPUD_IRQ_BASE + 0x08);
    *irq_clear = 0xFF;

    // Disable all interrupts initially
    volatile uint32_t* irq_enable = (volatile uint32_t*)(SPUD_IRQ_BASE + 0x04);
    *irq_enable = 0x00;
}

void irq_enable(uint8_t irq_num) {
    if(irq_num >= 8) return;

    volatile uint32_t* irq_enable = (volatile uint32_t*)(SPUD_IRQ_BASE + 0x04);
    *irq_enable |= (1 << irq_num);
}

void irq_disable(uint8_t irq_num) {
    if(irq_num >= 8) return;

    volatile uint32_t* irq_enable = (volatile uint32_t*)(SPUD_IRQ_BASE + 0x04);
    *irq_enable &= ~(1 << irq_num);
}

void irq_set_handler(uint8_t irq_num, irq_handler_t handler) {
    if(irq_num >= 8) return;

    irq_handlers[irq_num] = handler;
}

void irq_global_enable(void) {
    asm volatile ("csrs mstatus, %0" :: "r" (0x8)); // Set MIE bit
}

void irq_global_disable(void) {
    asm volatile ("csrc mstatus, %0" :: "r" (0x8)); // Clear MIE bit
}

// Generic IRQ handler (would be called by interrupt vector)
void __attribute__((weak)) irq_handler(void) {
    volatile uint32_t* irq_pending = (volatile uint32_t*)(SPUD_IRQ_BASE + 0x00);
    uint32_t pending = *irq_pending;

    for(int i = 0; i < 8; i++) {
        if((pending & (1 << i)) && irq_handlers[i]) {
            irq_handlers[i]();

            // Clear the interrupt
            volatile uint32_t* irq_clear = (volatile uint32_t*)(SPUD_IRQ_BASE + 0x08);
            *irq_clear = (1 << i);
        }
    }
}
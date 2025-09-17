#ifndef SPUDKIT_IRQ_H
#define SPUDKIT_IRQ_H

#include <stdint.h>

// IRQ numbers (these would match your IRQ controller)
#define IRQ_TIMER   0
#define IRQ_UART    1
#define IRQ_GPIO    2
#define IRQ_SPI     3
#define IRQ_DISPLAY 4

// IRQ callback type
typedef void (*irq_handler_t)(void);

// IRQ initialization
void irq_init(void);

// IRQ management functions
void irq_enable(uint8_t irq_num);
void irq_disable(uint8_t irq_num);
void irq_set_handler(uint8_t irq_num, irq_handler_t handler);

// Global interrupt control
void irq_global_enable(void);
void irq_global_disable(void);

#endif // SPUDKIT_IRQ_H
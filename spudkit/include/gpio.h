#ifndef SPUDKIT_GPIO_H
#define SPUDKIT_GPIO_H

#include <stdint.h>

// GPIO direction modes
#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

// GPIO initialization
void gpio_init(void);

// Basic GPIO functions
void gpio_set_direction(uint8_t pin, uint8_t direction);
void gpio_write(uint8_t pin, uint8_t value);
uint8_t gpio_read(uint8_t pin);
void gpio_toggle(uint8_t pin);

// Bulk operations
void gpio_write_mask(uint32_t mask, uint32_t value);
uint32_t gpio_read_all(void);

#endif // SPUDKIT_GPIO_H
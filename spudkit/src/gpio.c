#include "gpio.h"
#include "spudkit.h"

void gpio_init(void) {
    // TODO: Initialize GPIO controller
}

void gpio_set_direction(uint8_t pin, uint8_t direction) {
    volatile uint32_t* gpio_dir = (volatile uint32_t*)(SPUD_GPIO_BASE + 0x00);
    uint32_t current = *gpio_dir;

    if (direction == GPIO_OUTPUT) {
        current |= (1 << pin);
    } else {
        current &= ~(1 << pin);
    }

    *gpio_dir = current;
}

void gpio_write(uint8_t pin, uint8_t value) {
    if (value) {
        volatile uint32_t* gpio_set = (volatile uint32_t*)(SPUD_GPIO_BASE + 0x0C);
        *gpio_set = (1 << pin);
    } else {
        volatile uint32_t* gpio_clr = (volatile uint32_t*)(SPUD_GPIO_BASE + 0x10);
        *gpio_clr = (1 << pin);
    }
}

uint8_t gpio_read(uint8_t pin) {
    volatile uint32_t* gpio_input = (volatile uint32_t*)(SPUD_GPIO_BASE + 0x04);
    return (*gpio_input >> pin) & 1;
}

void gpio_toggle(uint8_t pin) {
    uint8_t current = gpio_read(pin);
    gpio_write(pin, !current);
}

void gpio_write_mask(uint32_t mask, uint32_t value) {
    volatile uint32_t* gpio_output = (volatile uint32_t*)(SPUD_GPIO_BASE + 0x08);
    uint32_t current = *gpio_output;
    current = (current & ~mask) | (value & mask);
    *gpio_output = current;
}

uint32_t gpio_read_all(void) {
    volatile uint32_t* gpio_input = (volatile uint32_t*)(SPUD_GPIO_BASE + 0x04);
    return *gpio_input;
}
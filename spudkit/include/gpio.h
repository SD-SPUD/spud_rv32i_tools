#ifndef SPUDKIT_GPIO_H
#define SPUDKIT_GPIO_H

#include <stdint.h>

// GPIO direction modes
#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

// Arcade button pin definitions (connected to GPIO pins 0-9)
#define ARCADE_BUTTON_0  0
#define ARCADE_BUTTON_1  1
#define ARCADE_BUTTON_2  2
#define ARCADE_BUTTON_3  3
#define ARCADE_BUTTON_4  4
#define ARCADE_BUTTON_5  5
#define ARCADE_BUTTON_6  6
#define ARCADE_BUTTON_7  7
#define ARCADE_BUTTON_8  8
#define ARCADE_BUTTON_9  9

// ALT arcade button pin names
#define ARCADE_BUTTON_LEFT 0
#define ARCADE_BUTTON_DOWN  1
#define ARCADE_BUTTON_RIGHT  2
#define ARCADE_BUTTON_UP  3
#define ARCADE_BUTTON_X  4
#define ARCADE_BUTTON_Y  5
#define ARCADE_BUTTON_A  6
#define ARCADE_BUTTON_B  7
#define ARCADE_BUTTON_START  8
#define ARCADE_BUTTON_SELECT  9

// Arcade button mask (lower 10 bits)
#define ARCADE_BUTTON_MASK 0x3FF

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

// Arcade button functions
void arcade_init(void);
uint8_t arcade_button_read(uint8_t button);
uint16_t arcade_read_all(void);

#endif // SPUDKIT_GPIO_H

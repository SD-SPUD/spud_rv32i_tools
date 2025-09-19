#include "led.h"
#include "spudkit.h"

void led_init(void) {
    // initialize led controller - turn off both leds
    volatile uint32_t* led_ctrl = (volatile uint32_t*)(SPUD_LED_BASE + LED_REG_CTRL);
    *led_ctrl = 0x10; // both leds off
}

void led_set(uint8_t led_num, uint8_t state) {
    volatile uint32_t* led_ctrl = (volatile uint32_t*)(SPUD_LED_BASE + LED_REG_CTRL);
    uint32_t current = *led_ctrl;

    if(led_num == 0) {
        // control led0 (bit 0)
        if(state) {
            current |= (1 << LED0_BIT);   // set bit 0
        } else {
            current &= ~(1 << LED0_BIT);  // clear bit 0
        }
    } else if(led_num == 1) {
        // control led1 (bit 1)
        if(state) {
            current |= (1 << LED1_BIT);   // set bit 1
        } else {
            current &= ~(1 << LED1_BIT);  // clear bit 1
        }
    }

    *led_ctrl = current;
}

void led_toggle(uint8_t led_num) {
    volatile uint32_t* led_ctrl = (volatile uint32_t*)(SPUD_LED_BASE + LED_REG_CTRL);
    uint32_t current = *led_ctrl;

    if(led_num == 0) {
        current ^= (1 << LED0_BIT);  // toggle bit 0
    } else if(led_num == 1) {
        current ^= (1 << LED1_BIT);  // toggle bit 1
    }

    *led_ctrl = current;
}

void led_set_both(uint8_t led0_state, uint8_t led1_state) {
    volatile uint32_t* led_ctrl = (volatile uint32_t*)(SPUD_LED_BASE + LED_REG_CTRL);
    uint32_t value = 0;

    if(led0_state) value |= (1 << LED0_BIT);
    if(led1_state) value |= (1 << LED1_BIT);

    *led_ctrl = value;
}

uint8_t led_get(uint8_t led_num) {
    volatile uint32_t* led_ctrl = (volatile uint32_t*)(SPUD_LED_BASE + LED_REG_CTRL);
    uint32_t current = *led_ctrl;

    if(led_num == 0) {
        return (current & (1 << LED0_BIT)) ? 1 : 0;
    } else if(led_num == 1) {
        return (current & (1 << LED1_BIT)) ? 1 : 0;
    }

    return 0;
}

void led_pattern(uint8_t pattern) {
    // set led pattern directly from lower 2 bits
    // bit 0 = led0, bit 1 = led1
    volatile uint32_t* led_ctrl = (volatile uint32_t*)(SPUD_LED_BASE + LED_REG_CTRL);
    *led_ctrl = pattern & 0x03; // mask to keep only bits 0-1
}

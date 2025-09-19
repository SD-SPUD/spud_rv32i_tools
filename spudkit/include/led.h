#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "spudkit.h"

// led register offsets
#define LED_REG_CTRL    0x00    // led control register (bits 0-1 for led0 and led1)

// led bit definitions
#define LED0_BIT        0       // bit 0 controls led0
#define LED1_BIT        1       // bit 1 controls led1

// led control values
#define LED_OFF         0
#define LED_ON          1

// led control functions
void led_init(void);
void led_set(uint8_t led_num, uint8_t state);
void led_toggle(uint8_t led_num);
void led_set_both(uint8_t led0_state, uint8_t led1_state);
uint8_t led_get(uint8_t led_num);
void led_pattern(uint8_t pattern);

#endif // LED_H
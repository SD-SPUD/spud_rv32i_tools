#include "spudkit.h"
#include <stdint.h>

#define LED_REG (*(volatile uint32_t *)(SPUD_LED_BASE))

// crude delay loop for simulation
static void sim_delay(int cycles) {
    for (volatile int i = 0; i < cycles * 1000; i++);
}

int main() {
    spudkit_init();

    sim_puts("=== LED Controller Simulation Test ===\n");
    uart_puts("LED Controller Simulation Test\r\n");

    // make sure we start with LEDs off
    LED_REG = 0x0;
    sim_puts("LEDs off (00)\n");
    sim_delay(1);

    // test LED pattern 01
    LED_REG = 0x1;
    sim_puts("LED pattern: 01\n");
    sim_delay(1);

    // test LED pattern 10
    LED_REG = 0x2;
    sim_puts("LED pattern: 10\n");
    sim_delay(1);

    // test LED pattern 11
    LED_REG = 0x3;
    sim_puts("LED pattern: 11\n");
    sim_delay(1);

    // loop through patterns continuously
    sim_puts("Entering LED pattern loop...\n");
    while (1) {
        for (uint32_t pattern = 0; pattern < 4; pattern++) {
            LED_REG = pattern;
            sim_puts("LED pattern written.\n");
            sim_delay(2000);
        }
    }

    return 0;
}


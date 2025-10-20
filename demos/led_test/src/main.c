#include "spudkit.h"

int main() {
    // initialize the spudkit library
    spudkit_init();
    volatile uint32_t* led_ctrl = (volatile uint32_t*)0x95000000;
    while (1) {
        *led_ctrl = 1;
        for (int i = 0; i < 1000000; i++) {
            __asm__("nop");
        }
        *led_ctrl = 0;
    }
    return 0;
}

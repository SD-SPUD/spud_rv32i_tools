#include "spudkit.h"

int main() {
    // initialize the spudkit library
    spudkit_init();
    while (1) {
        volatile uint32_t* led_ctrl = (volatile uint32_t*)0x95000000;
        *led_ctrl = 3;
    }
    return 0;
}

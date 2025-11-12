#include "spudkit.h"

void sleepx(int x){
    for(int i = 0; i < x; i++) {
        __asm__ volatile("nop");
    }
}

int main() {
    // initialize the spudkit library
    spudkit_init();
    volatile uint32_t* led_ctrl = (volatile uint32_t*)0x95000000;
    int count = 0;
    while (1) {
        *led_ctrl = 0;
        sleepx(2000000);
        *led_ctrl = 1;
        sleepx(2000000);
    }
    return 0;
}

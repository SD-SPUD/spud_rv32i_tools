#include "spudkit.h"

int main() {
    // initialize the spudkit library
    spudkit_init();
    // initialize led controller
    led_init(); // TODO: move into spudkit
    while(1) {
        led_set(0, LED_ON);
        led_set(0, LED_OFF);
    }

    return 0;
}

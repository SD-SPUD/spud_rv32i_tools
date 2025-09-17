#include "spudkit.h"

int main() {
    // initialize spudkit library
    spudkit_init();

    sim_puts("hello world!!!\n");
    uart_puts("\r\nhello world from spudkit!!!\r\n");

    // set gpio pin 0 as output for led
    gpio_set_direction(0, GPIO_OUTPUT);

    while(1) {
        // turn led on
        gpio_write(0, 1);
        uart_puts("LED 0 ON\r\n");
        delay_ms(1000);

        // turn led off
        gpio_write(0, 0);
        uart_puts("LED 0 OFF\r\n");
        delay_ms(1000);
    }
    return 0;
}

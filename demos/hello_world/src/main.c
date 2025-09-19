#include "spudkit.h"

int main() {
    // initialize spudkit library
    spudkit_init();

    sim_puts("hello world!!!\n");
    uart_puts("\r\nhello world from spudkit!!!\r\n");

    // set gpio pin 0 as output for led
    gpio_set_direction(0, GPIO_OUTPUT);


    for(ffloat i = FFLOAT(0); i < FFLOAT(6.2); i+=FFLOAT(0.2)) {
        uart_puts("sin(");
        ffprint(i);
        uart_puts(") = :");
        ffprint(ffsin(i));
        uart_putc('\n');
    }
    return 0;

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

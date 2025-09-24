#include "spudkit.h"

int main() {
    // initialize spudkit library
    spudkit_init();

    sim_puts("hello world!!!\n");
    uart_puts("\r\nhello world\r\n");

    while(1) {
        // turn led on gpio_write(0, 1);
        uart_puts("loop\r\n");
        delay_ms(1000);
    }
    return 0;
}

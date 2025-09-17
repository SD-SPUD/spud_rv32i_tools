#include "spudkit.h"

void spudkit_init(void) {
    // Initialize all peripheral subsystems
    uart_init(1000000);  // 1Mbaud default
    gpio_init();
    timer_init();
    spi_init(1000000, SPI_MODE_0);  // 1MHz default
    irq_init();
    display_init();
}
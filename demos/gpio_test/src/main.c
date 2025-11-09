#include "spudkit.h"

int gpio_test_main() {
    // initialize spudkit library
    spudkit_init();

    sim_puts("SpudKit GPIO Test Demo\n");
    sim_puts("======================\n\n");
    uart_puts("\r\nSpudKit GPIO Test Demo via UART\r\n");
    uart_puts("==============================\r\n\r\n");

    sim_puts("Setting up GPIO pins...\n");
    uart_puts("Setting up GPIO pins...\r\n");
    gpio_set_direction(0, GPIO_OUTPUT);
    gpio_set_direction(1, GPIO_OUTPUT);
    gpio_set_direction(2, GPIO_INPUT);
    gpio_set_direction(3, GPIO_INPUT);

    sim_puts("Testing GPIO writes...\n");
    uart_puts("Testing GPIO writes...\r\n");
    gpio_write(0, 1);
    gpio_write(1, 0);
    sim_puts("GPIO pin 0 set to HIGH\n");
    sim_puts("GPIO pin 1 set to LOW\n");
    uart_puts("GPIO pin 0 set to HIGH\r\n");
    uart_puts("GPIO pin 1 set to LOW\r\n");

    sim_puts("\nTesting GPIO reads...\n");
    uart_puts("\r\nTesting GPIO reads...\r\n");
    uint8_t pin2_value = gpio_read(2);
    uint8_t pin3_value = gpio_read(3);

    sim_puts("GPIO pin 2 value: ");
    sim_putc('0' + pin2_value);
    sim_puts("\n");
    uart_puts("GPIO pin 2 value: ");
    uart_putc('0' + pin2_value);
    uart_puts("\r\n");

    sim_puts("GPIO pin 3 value: ");
    sim_putc('0' + pin3_value);
    sim_puts("\n");
    uart_puts("GPIO pin 3 value: ");
    uart_putc('0' + pin3_value);
    uart_puts("\r\n");

    sim_puts("\nToggling output pins...\n");
    uart_puts("\r\nToggling output pins...\r\n");
    for (int i = 0; i < 5; i++) {
        gpio_write(0, i % 2);
        gpio_write(1, (i + 1) % 2);

        sim_puts("Toggle ");
        sim_putc('0' + i);
        sim_puts(" - Pin 0: ");
        sim_putc('0' + (i % 2));
        sim_puts(", Pin 1: ");
        sim_putc('0' + ((i + 1) % 2));
        sim_puts("\n");

        uart_puts("Toggle ");
        uart_putc('0' + i);
        uart_puts(" - Pin 0: ");
        uart_putc('0' + (i % 2));
        uart_puts(", Pin 1: ");
        uart_putc('0' + ((i + 1) % 2));
        uart_puts("\r\n");

        delay_ms(500);
    }

    sim_puts("\nTesting bulk GPIO operations...\n");
    uart_puts("\r\nTesting bulk GPIO operations...\r\n");

    // test gpio_toggle function
    for(int i = 0; i < 10; i++) {
        gpio_toggle(0);
        gpio_toggle(1);
        uart_puts("Toggled pins 0 and 1\r\n");
        delay_ms(200);
    }

    // test bulk operations
    gpio_write_mask(0x03, 0x03); // set pins 0 and 1 high
    uart_puts("Set pins 0,1 HIGH using mask\r\n");
    delay_ms(1000);

    gpio_write_mask(0x03, 0x00); // set pins 0 and 1 low
    uart_puts("Set pins 0,1 LOW using mask\r\n");
    delay_ms(1000);

    uint32_t all_pins = gpio_read_all();
    uart_puts("All GPIO pins state: 0x");
    sim_put_hex8(all_pins & 0xFF);
    uart_puts("\r\n");

    sim_puts("\nSpudKit GPIO test complete!\n");
    uart_puts("\r\nSpudKit GPIO test complete!\r\n");
    return 0;
}
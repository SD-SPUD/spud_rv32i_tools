#include "spudkit.h"

int main() {
    // initialize the spudkit library
    spudkit_init();
    uart_puts("\r\nSpudKit LED Test Demo\r\n");
    uart_puts("====================\r\n\r\n");

    // initialize led controller
    led_init();
    uart_puts("led controller initialized\r\n");

    // test individual led control
    uart_puts("testing individual led control...\r\n");

    while(1) {
    // turn on led0 only (pattern 01)
        uart_puts("led0 on, led1 off (pattern 01)\r\n");
        led_set(0, LED_ON);
        delay_ms(1000);
        led_set(0, LED_OFF);
    }


    // turn on led1 only (pattern 10)
    uart_puts("led0 off, led1 on (pattern 10)\r\n");
    led_set(0, LED_OFF);
    led_set(1, LED_ON);
    delay_ms(1000);

    // turn on both leds (pattern 11)
    uart_puts("both leds on (pattern 11)\r\n");
    led_set_both(LED_ON, LED_ON);
    delay_ms(1000);

    // turn off both leds (pattern 00)
    uart_puts("both leds off (pattern 00)\r\n");
    led_set_both(LED_OFF, LED_OFF);
    delay_ms(1000);

    // test led patterns directly
    uart_puts("testing led patterns...\r\n");

    uint8_t patterns[] = {0x01, 0x02, 0x03, 0x00}; // 01, 10, 11, 00
    const char* pattern_names[] = {"01 (led0 only)", "10 (led1 only)", "11 (both leds)", "00 (no leds)"};

    for(int i = 0; i < 4; i++) {
        uart_puts("pattern ");
        uart_puts(pattern_names[i]);
        uart_puts("\r\n");
        led_pattern(patterns[i]);
        delay_ms(800);
    }

    // test led toggle functionality
    uart_puts("testing led toggle...\r\n");
    led_set_both(LED_OFF, LED_OFF); // start with both off

    for(int i = 0; i < 8; i++) {
        uart_puts("toggle led0\r\n");
        led_toggle(0);
        delay_ms(500);

        uart_puts("toggle led1\r\n");
        led_toggle(1);
        delay_ms(500);
    }

    // blinking pattern demonstration
    uart_puts("blinking pattern demo...\r\n");

    for(int cycle = 0; cycle < 5; cycle++) {
        // alternating pattern
        led_pattern(0x01); // led0 on
        delay_ms(200);
        led_pattern(0x02); // led1 on
        delay_ms(200);
    }

    // final status check
    uart_puts("final led status check:\r\n");
    uart_puts("led0 state: ");
    uart_put_dec(led_get(0));
    uart_puts("\r\n");
    uart_puts("led1 state: ");
    uart_put_dec(led_get(1));
    uart_puts("\r\n");

    uart_puts("led test complete!\r\n");

    // turn off all leds when done
    led_set_both(LED_OFF, LED_OFF);

    return 0;
}

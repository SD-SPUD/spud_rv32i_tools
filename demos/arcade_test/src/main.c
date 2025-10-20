#include "spudkit.h"

// Button name lookup table
const char* button_names[10] = {
    "LEFT ",   // Pin 0
    "DOWN ",   // Pin 1
    "RIGHT",   // Pin 2
    "UP   ",   // Pin 3
    "X    ",   // Pin 4
    "Y    ",   // Pin 5
    "A    ",   // Pin 6
    "B    ",   // Pin 7
    "START",   // Pin 8
    "SELCT"    // Pin 9
};
/*

int main() {
    // initialize spudkit library
    spudkit_init();

    uart_puts("\r\nArcade Button Test - Enhanced Display\r\n");
    uart_puts("=====================================\r\n");
    uart_puts("Button mapping:\r\n");
    uart_puts("  LEFT=0  DOWN=1  RIGHT=2  UP=3\r\n");
    uart_puts("  X=4     Y=5     A=6      B=7\r\n");qq
    uart_puts("  START=8 SELECT=9\r\n\r\n");

    // Initialize arcade buttons (set pins 0-9 as inputs)
    arcade_init();
    uart_puts("Arcade buttons initialized\r\n");
    uart_puts("Press buttons to test (1=pressed, 0=released)\r\n\r\n");

    uint16_t last_buttons = 0xFFFF; // Force initial display

    while(1) {
        // Read all button states
        uint16_t buttons = arcade_read_all();

        // Only update display when buttons change
        if (buttons != last_buttons) {
            uart_cursor_home();
            uart_puts("Raw: 0x");
            uart_put_hex16(buttons);
            uart_puts(" Binary: ");

            // Print binary representation (LSB first)
            for (int i = 9; i >= 0; i--) {
                uart_putc('0' + ((buttons >> i) & 1));
            }
            uart_puts("\r\n\r\n");

            // Print named buttons in a nice format
            uart_puts("Button States:\r\n");
            uart_puts("┌──────┬───────┬──────┬───────┐\r\n");
            uart_puts("│ LEFT │ DOWN  │RIGHT │  UP   │\r\n");
            uart_puts("│  ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_LEFT) & 1));
            uart_puts("   │   ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_DOWN) & 1));
            uart_puts("   │  ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_RIGHT) & 1));
            uart_puts("   │   ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_UP) & 1));
            uart_puts("   │\r\n");
            uart_puts("├──────┼───────┼──────┼───────┤\r\n");
            uart_puts("│  X   │   Y   │  A   │   B   │\r\n");
            uart_puts("│  ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_X) & 1));
            uart_puts("   │   ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_Y) & 1));
            uart_puts("   │  ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_A) & 1));
            uart_puts("   │   ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_B) & 1));
            uart_puts("   │\r\n");
            uart_puts("├──────┴───────┼──────┴───────┤\r\n");
            uart_puts("│    START     │    SELECT    │\r\n");
            uart_puts("│      ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_START) & 1));
            uart_puts("       │      ");
            uart_putc('0' + ((buttons >> ARCADE_BUTTON_SELECT) & 1));
            uart_puts("       │\r\n");
            uart_puts("└──────────────┴──────────────┘\r\n\r\n");

            // List pressed buttons
            uart_puts("Pressed: ");
            int any_pressed = 0;
            for (int i = 0; i < 10; i++) {
                if ((buttons >> i) & 1) {
                    if (any_pressed) uart_puts(", ");
                    uart_puts(button_names[i]);
                    any_pressed = 1;
                }
            }
            if (!any_pressed) uart_puts("(none)");
            uart_puts("\r\n");

            last_buttons = buttons;
        }

        // Small delay to prevent overwhelming the UART
        for (volatile int i = 0; i < 10000; i++);
    }

    return 0;
}

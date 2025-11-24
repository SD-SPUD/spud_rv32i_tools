// SAMPLE - Minimal SpudKit API demo
#include "spudkit.h"

// Button macro
#define BUTTON_PRESSED(b, p) (p & (1 << (b)))

int sample_main() {
    spudkit_init();
    arcade_init();

    uint8_t x = 32, y = 32;
    uint16_t last_btn = arcade_read_all();
    uint8_t show_text = 1;  // Show text by default

    uart_puts("\r\nSAMPLE DEMO\r\n");
    uart_puts("A=toggle text/trail, D-pad=move\r\n");

    while (1) {
        uint16_t btn = arcade_read_all();
        uint16_t pressed = btn & ~last_btn;

        // A - toggle between text mode and trail mode
        if (BUTTON_PRESSED(ARCADE_BUTTON_A, pressed)) {
            show_text = !show_text;
            if (!show_text) display_clear(COLOR_BLACK);  // Clear when entering trail mode
        }

        // Move cursor while buttons held
        if (BUTTON_PRESSED(ARCADE_BUTTON_UP, btn))    y = (y < 63) ? y + 1 : 63;
        if (BUTTON_PRESSED(ARCADE_BUTTON_DOWN, btn))  y = (y > 0) ? y - 1 : 0;
        if (BUTTON_PRESSED(ARCADE_BUTTON_LEFT, btn))  x = (x < 63) ? x + 1 : 63;
        if (BUTTON_PRESSED(ARCADE_BUTTON_RIGHT, btn)) x = (x > 0) ? x - 1 : 0;

	// exit the game
        if (BUTTON_PRESSED(ARCADE_BUTTON_SELECT, pressed)) break; 


        // Draw frame
        if (show_text) {
            display_clear(COLOR_BLACK);
            display_draw_string(0, 0, "HELLO", COLOR_YELLOW, COLOR_BLACK);       // 8px font
            display_draw_string_small(0, 10, "World", COLOR_GREEN, COLOR_BLACK); // 6px font
            display_draw_string_scaled(0, 20, "BIG", COLOR_RED, COLOR_BLACK, 2); // 2x scaled
        }

        display_set_pixel(x, y, COLOR_WHITE);
        display_update();

        last_btn = btn;
        for (volatile int i = 0; i < 10000; i++);
    }
    return 0;
}

int main() {
    sample_main();
    return 0;
}

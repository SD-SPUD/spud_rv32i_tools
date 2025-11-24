#include "spudkit.h"
#include "menu.h"

int main(void) {
    // Initialize SpudKit
    spudkit_init();

    uart_puts("\r\n");
    uart_puts("=========================================\r\n");
    uart_puts("           SPUD GAME LAUNCHER            \r\n");
    uart_puts("=========================================\r\n");
    uart_puts("\r\n");
    uart_puts("Welcome to the SPUD Demo Launcher!\r\n");
    uart_puts("\r\n");
    uart_puts("Controls:\r\n");
    uart_puts("  UP/DOWN or W/S  - Navigate menu\r\n");
    uart_puts("  A or ENTER      - Select game\r\n");
    uart_puts("  SELECT (in-game)- Return to menu\r\n");
    uart_puts("\r\n");
    uart_puts("Available games:\r\n");
    uart_puts("  - SNAKE\r\n");
    uart_puts("  - TETRIS\r\n");
    uart_puts("  - CHESS\r\n");
    uart_puts("  - SPUDMAN\r\n");
    uart_puts("  - SANDBOX\r\n");
    uart_puts("\r\n");

    // Initialize menu
    menu_state_t menu;
    menu_init(&menu);

    // Main launcher loop
    while (1) {
        // Draw menu
        menu_draw(&menu);

        // Update menu and check for selection
        int selected = menu_update(&menu);

        if (selected >= 0) {
            // Clear display before launching game
            display_clear(COLOR_BLACK);
            display_update();

            // Launch selected game
            uart_puts("\r\nLaunching game...\r\n\r\n");

            switch (selected) {
                case GAME_SNAKE:
                    uart_puts("Starting SNAKE...\r\n");
                    snake_main();
                    break;
                case GAME_TETRIS:
                    uart_puts("Starting TETRIS...\r\n");
                    tetris_main();
                    break;
                case GAME_CHESS:
                    uart_puts("Starting CHESS...\r\n");
                    chess_main();
                    break;
                case GAME_SPUDMAN:
                    uart_puts("Starting SPUDMAN...\r\n");
                    spudman_main();
                    break;
                case GAME_DONUT:
                    uart_puts("Starting SANDBOX...\r\n");
                    sample_main();
                    break;
            }

            // Game exited, return to menu
            uart_puts("\r\n");
            uart_puts("Returning to launcher...\r\n\r\n");

            // Reinitialize arcade buttons (in case game changed state)
            arcade_init();
            menu.last_buttons = arcade_read_all();

            // Small delay to debounce
            for (volatile int i = 0; i < 100000; i++);
        }

        // Small delay to prevent overwhelming the system
        for (volatile int i = 0; i < 10000; i++);
    }

    return 0;
}

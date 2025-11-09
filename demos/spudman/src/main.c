#include "spudkit.h"
#include "game.h"
#include "controls.h"

int main(void) {
    // Initialize SpudKit
    spudkit_init();
    display_init();

    uart_puts("\r\n");
    uart_puts("=========================================\r\n");
    uart_puts("              SPUDMAN! 🥔👻              \r\n");
    uart_puts("=========================================\r\n");
    uart_puts("\r\n");
    uart_puts("A potato's quest to eat all the sprouts!\r\n");
    uart_puts("\r\n");
    uart_puts("Controls:\r\n");
    uart_puts("  Arcade: UP/DOWN/LEFT/RIGHT buttons\r\n");
    uart_puts("  Keyboard: W/A/S/D keys\r\n");
    uart_puts("  Q - Quit\r\n");
    uart_puts("\r\n");
    uart_puts("Rules:\r\n");
    uart_puts("  - Eat all green sprouts (pellets)\r\n");
    uart_puts("  - Avoid the ghosts!\r\n");
    uart_puts("  - Big sprouts = power pellets\r\n");
    uart_puts("  - Power pellets let you eat ghosts!\r\n");
    uart_puts("  - 3 lives to start\r\n");
    uart_puts("\r\n");
    uart_puts("Starting game...\r\n\r\n");

    // Initialize game
    game_state_t game;
    game_init(&game);

    controls_init();

    // Main game loop
    while (1) {
        // Handle input
        controls_update(&game);

        // Update game logic
        game_update(&game);

        // Render frame
        display_clear(COLOR_BLACK);
        game_draw(&game);
        display_update();

        // Frame delay
        for (volatile int i = 0; i < 100000; i++);
    }

    return 0;
}

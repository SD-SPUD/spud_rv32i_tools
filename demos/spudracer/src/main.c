#include "spudkit.h"
#include "game.h"
#include "track.h"
#include "controls.h"

int main(void) {
    // Initialize SpudKit
    spudkit_init();

    uart_puts("\r\n");
    uart_puts("=========================================\r\n");
    uart_puts("    SPUDRACER - Speed Racer @ UCF\r\n");
    uart_puts("=========================================\r\n");
    uart_puts("\r\n");
    uart_puts("Controls:\r\n");
    uart_puts("  Arcade Buttons:\r\n");
    uart_puts("    LEFT/RIGHT - Change lanes\r\n");
    uart_puts("    A Button   - Boost (brief invincibility)\r\n");
    uart_puts("  Keyboard:\r\n");
    uart_puts("    A/D        - Change lanes\r\n");
    uart_puts("    SPACE      - Boost\r\n");
    uart_puts("    Q          - Quit\r\n");
    uart_puts("\r\n");
    uart_puts("Features:\r\n");
    uart_puts("  - Dodge bot cars in 3 lanes\r\n");
    uart_puts("  - Pass UCF landmarks:\r\n");
    uart_puts("    * Reflection Pond with fountain\r\n");
    uart_puts("    * HEC 438 building\r\n");
    uart_puts("    * American flag on lawn\r\n");
    uart_puts("    * Palm trees (it's Florida!)\r\n");
    uart_puts("  - Score based on distance\r\n");
    uart_puts("  - Collisions slow you down\r\n");
    uart_puts("\r\n");
    uart_puts("Starting race...\r\n\r\n");

    // Initialize game modules
    game_state_t game;
    game_init(&game);

    track_init();
    controls_init();

    // Main game loop
    while (1) {
        // Handle input
        controls_update(&game);

        // Update game logic
        game_update(&game);

        // Render frame
        display_clear(COLOR_BLACK);
        track_draw(&game);
        track_draw_landmarks(&game);
        game_draw(&game);
        display_update();

        // Frame delay (adjust as needed)
        for (volatile int i = 0; i < 50000; i++);
    }

    return 0;
}

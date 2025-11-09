#include "spudkit.h"
#include "game.h"
#include "controls.h"

int snake_main() {
    uart_puts("welcome to snake\r\n");

    game_state_t game;
    game_init(&game);

    controls_init();

    uart_puts("snake game initialized\r\n");
    uart_puts("use WASD or arcade buttons to move\r\n");
    uart_puts("press Y button or R to reset the game\r\n");
    uart_puts("press SELECT button to exit to menu\r\n");

    while(1) {
        controls_update(&game);

        // check if user wants to exit to menu
        if (game.exitToMenu) {
            uart_puts("exiting to menu...\r\n");
            break;
        }

        // only update game if not game over
        if (!game.gameOver) {
            game_update(&game);
        }

        game_draw(&game);

        // small delay to control game speed (adjust for desired difficulty)
        for (volatile int i = 0; i < 50000; i++);
    }

    return 0;
}

#include "spudkit.h"
#include "game.h"
#include "controls.h"

int snake_main() {
    uart_puts("welcome to snake\r\n");

    game_state_t game;
    snake_game_init(&game);

    snake_controls_init();

    uart_puts("snake game initialized\r\n");
    uart_puts("use WASD or arcade buttons to move\r\n");
    uart_puts("press Y button or R to reset the game\r\n");
    uart_puts("press SELECT button to exit to menu\r\n");

    while(1) {
        snake_controls_update(&game);

        // check if user wants to exit to menu
        if (game.exitToMenu) {
            uart_puts("exiting to menu...\r\n");
            break;
        }

        // only update game if not game over
        if (!game.gameOver) {
            snake_game_update(&game);
        }

        snake_game_draw(&game);

        // small delay to control game speed (adjust for desired difficulty)
        for (volatile int i = 0; i < 50000; i++);
    }

    return 0;
}

int main() {
    spudkit_init();
    return snake_main();
}

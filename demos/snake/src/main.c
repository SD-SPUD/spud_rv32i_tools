#include "spudkit.h"
#include "game.h"
#include "controls.h"

int main() {
    spudkit_init();
    uart_puts("welcome to snake\r\n");

    game_state_t game;
    game_init(&game);

    controls_init();

    uart_puts("snake game initialized\r\n");
    uart_puts("use WASD or arcade buttons to move\r\n");
    uart_puts("press Y button or R to reset the game\r\n");

    while(1) {
        controls_update(&game);

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

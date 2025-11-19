#include "spudkit.h"
#include "board.h"
#include "game.h"
#include "controls.h"

int tetris_main() {
    uart_puts("welcome to tetris!\r\n");

    tetris_game_state_t game;
    tetris_game_init(&game);

    tetris_board_init();

    tetris_controls_init();

    uart_puts("tetris game initialized\r\n");
    uart_puts("controls:\r\n");
    uart_puts("  LEFT/RIGHT or A/D - move piece\r\n");
    uart_puts("  UP or W or Space - rotate piece\r\n");
    uart_puts("  DOWN or S - soft drop (faster fall)\r\n");
    uart_puts("  B button or Enter - hard drop (instant)\r\n");
    uart_puts("  Y button - reset game\r\n");
    uart_puts("  SELECT button - exit to menu\r\n");

    uint16_t last_buttons = arcade_read_all();

    while(1) {
        // check if user wants to exit to menu
        if (game.exitToMenu) {
            uart_puts("exiting to menu...\r\n");
            break;
        }

        // check for Y button press (reset game)
        uint16_t buttons = arcade_read_all();
        uint16_t button_pressed = buttons & ~last_buttons;  // detect rising edge

        if (button_pressed & (1 << ARCADE_BUTTON_Y)) {
            // reset the game
            tetris_game_init(&game);
            uart_puts("game reset!\r\n");
        }

        last_buttons = buttons;

        // update game logic
        tetris_controls_update(&game);

        // draw everything
        tetris_board_draw();
        tetris_game_draw_board(&game);
        tetris_game_draw_active_piece(&game);
        tetris_game_draw_score(&game);

        // show game over message
        if (game.game_over) {
            // Simple game over indicator - flash the screen
            static uint8_t flash_counter = 0;
            flash_counter++;
            if ((flash_counter / 30) % 2 == 0) {
                // Draw a border to indicate game over
                for (uint8_t x = 0; x < 40; x++) {
                    display_set_pixel(x, 0, COLOR_RED);
                    display_set_pixel(x, 63, COLOR_RED);
                }
                for (uint8_t y = 0; y < 64; y++) {
                    display_set_pixel(0, y, COLOR_RED);
                    display_set_pixel(39, y, COLOR_RED);
                }
            }
        }

        display_update();

        // small delay to control frame rate
        for (volatile int i = 0; i < 50000; i++);
    }
    return 0;
}

int main() {
    spudkit_init();
    return tetris_main();
}

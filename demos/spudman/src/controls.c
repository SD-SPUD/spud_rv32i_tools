#include "controls.h"

static uint16_t last_buttons = 0;

void controls_init(void) {
    arcade_init();
    last_buttons = arcade_read_all();
}

void controls_update(game_state_t* game) {
    // Read arcade buttons
    uint16_t buttons = arcade_read_all();
    uint16_t button_pressed = buttons & ~last_buttons;

    // Handle home screen - A button starts game
    if (game->game_state == GAME_STATE_HOME) {
        if (button_pressed & (1 << ARCADE_BUTTON_A)) {
            game_reset(game);
            uart_puts("Game started!\r\n");
        }
        last_buttons = buttons;
        return;
    }

    // Handle game over - A button returns to home screen
    if (game->game_state == GAME_STATE_GAME_OVER) {
        if (button_pressed & (1 << ARCADE_BUTTON_A)) {
            game_init(game);  // Return to home screen
            uart_puts("Returning to home screen...\r\n");
        }
        last_buttons = buttons;
        return;
    }

    // Y button resets game during play
    if (button_pressed & (1 << ARCADE_BUTTON_Y)) {
        game_reset(game);
        uart_puts("Game reset!\r\n");
    }

    // Queue direction change
    if (button_pressed & (1 << ARCADE_BUTTON_UP)) {
        game->spudman.next_direction = DIR_UP;
    }
    if (button_pressed & (1 << ARCADE_BUTTON_DOWN)) {
        game->spudman.next_direction = DIR_DOWN;
    }
    if (button_pressed & (1 << ARCADE_BUTTON_LEFT)) {
        game->spudman.next_direction = DIR_LEFT;
    }
    if (button_pressed & (1 << ARCADE_BUTTON_RIGHT)) {
        game->spudman.next_direction = DIR_RIGHT;
    }

    last_buttons = buttons;

    // UART keyboard controls
    if (uart_available()) {
        char c = uart_getc();

        // Handle home screen - A, Enter or Space starts game
        if (game->game_state == GAME_STATE_HOME) {
            if (c == 'a' || c == 'A' || c == '\r' || c == '\n' || c == ' ') {
                game_reset(game);
                uart_puts("Game started!\r\n");
            }
            return;  // Don't process other keys on home screen
        }

        // Handle game over - A, Enter or Space returns to home screen
        if (game->game_state == GAME_STATE_GAME_OVER) {
            if (c == 'a' || c == 'A' || c == '\r' || c == '\n' || c == ' ') {
                game_init(game);  // Return to home screen
                uart_puts("Returning to home screen...\r\n");
            }
            return;  // Don't process other keys on game over screen
        }

        // In-game controls
        {
            switch(c) {
                case 'w':
                case 'W':
                    game->spudman.next_direction = DIR_UP;
                    break;
                case 's':
                case 'S':
                    game->spudman.next_direction = DIR_DOWN;
                    break;
                case 'a':
                case 'A':
                    game->spudman.next_direction = DIR_LEFT;
                    break;
                case 'd':
                case 'D':
                    game->spudman.next_direction = DIR_RIGHT;
                    break;
                case 'r':
                case 'R':
                    // Reset game during play
                    game_reset(game);
                    uart_puts("Game reset!\r\n");
                    break;
                case 'q':
                case 'Q':
                    uart_puts("\r\nThanks for playing SPUDMAN!\r\n");
                    uart_exit(0);
                    break;
            }
        }
    }
}

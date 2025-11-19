#include "controls.h"

// previous button state for edge detection
static uint16_t last_buttons = 0;

void snake_controls_init(void) {
    // initialize arcade buttons
    arcade_init();
    last_buttons = arcade_read_all();
}

void snake_controls_update(game_state_t* game) {
    // read arcade buttons
    uint16_t buttons = arcade_read_all();
    uint16_t button_pressed = buttons & ~last_buttons;  // detect rising edge

    // handle directional input from arcade buttons
    if (button_pressed & (1 << ARCADE_BUTTON_LEFT)) {
        // prevent 180 degree turn
        if (game->dir != DIR_RIGHT) {
            game->dir = DIR_LEFT;
        }
    }
    if (button_pressed & (1 << ARCADE_BUTTON_RIGHT)) {
        if (game->dir != DIR_LEFT) {
            game->dir = DIR_RIGHT;
        }
    }
    if (button_pressed & (1 << ARCADE_BUTTON_UP)) {
        if (game->dir != DIR_DOWN) {
            game->dir = DIR_UP;
        }
    }
    if (button_pressed & (1 << ARCADE_BUTTON_DOWN)) {
        if (game->dir != DIR_UP) {
            game->dir = DIR_DOWN;
        }
    }

    // Y button resets the game
    if (button_pressed & (1 << ARCADE_BUTTON_Y)) {
        snake_game_init(game);
    }

    // SELECT button exits to menu
    if (button_pressed & (1 << ARCADE_BUTTON_SELECT)) {
        game->exitToMenu = true;
    }

    last_buttons = buttons;

    // check for UART input (WASD)
    if (uart_available()) {
        char c = uart_getc();

        switch(c) {
            case 'w':
            case 'W':
                if (game->dir != DIR_DOWN) {
                    game->dir = DIR_UP;
                }
                break;
            case 's':
            case 'S':
                if (game->dir != DIR_UP) {
                    game->dir = DIR_DOWN;
                }
                break;
            case 'a':
            case 'A':
                if (game->dir != DIR_RIGHT) {
                    game->dir = DIR_LEFT;
                }
                break;
            case 'd':
            case 'D':
                if (game->dir != DIR_LEFT) {
                    game->dir = DIR_RIGHT;
                }
                break;
            case 'r':
            case 'R':
                snake_game_init(game);
                break;
        }
    }
}

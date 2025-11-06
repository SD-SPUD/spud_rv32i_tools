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

    // UART keyboard controls (WASD)
    if (uart_available()) {
        char c = uart_getc();

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
            case 'q':
            case 'Q':
                uart_puts("\r\nThanks for playing SPUDMAN!\r\n");
                uart_exit(0);
                break;
        }
    }
}

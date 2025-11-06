#include "controls.h"

// Previous button state for edge detection
static uint16_t last_buttons = 0;

void controls_init(void) {
    arcade_init();
    last_buttons = arcade_read_all();
}

void controls_update(game_state_t* game) {
    // Read arcade buttons
    uint16_t buttons = arcade_read_all();
    uint16_t button_pressed = buttons & ~last_buttons;  // Detect rising edge

    // Lane switching with LEFT/RIGHT buttons
    if (button_pressed & (1 << ARCADE_BUTTON_LEFT)) {
        if (game->player.target_lane > 0) {
            game->player.target_lane--;
        }
    }
    if (button_pressed & (1 << ARCADE_BUTTON_RIGHT)) {
        if (game->player.target_lane < NUM_LANES - 1) {
            game->player.target_lane++;
        }
    }

    // Boost/trick with A button
    if (button_pressed & (1 << ARCADE_BUTTON_A)) {
        if (game->player.boost_timer == 0 && game->player.collision_timer == 0) {
            game->player.boost_timer = BOOST_DURATION;
        }
    }

    last_buttons = buttons;

    // Check for UART keyboard input (A/D for left/right, Space for boost)
    if (uart_available()) {
        char c = uart_getc();

        // Debug: show what key was pressed
        // uart_puts("Key: ");
        // uart_putc(c);
        // uart_puts("\r\n");

        switch(c) {
            case 'a':
            case 'A':
                uart_puts("LEFT\r\n");
                if (game->player.target_lane > 0) {
                    game->player.target_lane--;
                }
                break;
            case 'd':
            case 'D':
                uart_puts("RIGHT\r\n");
                if (game->player.target_lane < NUM_LANES - 1) {
                    game->player.target_lane++;
                }
                break;
            case ' ':  // Space bar for boost
                uart_puts("BOOST\r\n");
                if (game->player.boost_timer == 0 && game->player.collision_timer == 0) {
                    game->player.boost_timer = BOOST_DURATION;
                }
                break;
            case 'q':
            case 'Q':
                uart_puts("\r\nThanks for playing SPUDRACER!\r\n");
                uart_exit(0);
                break;
        }
    }
}

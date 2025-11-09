#include "controls.h"

// previous button state for edge detection
static uint16_t last_buttons = 0;

// timing for auto-repeat and soft drop
static uint32_t last_move_time = 0;
static uint32_t last_drop_time = 0;
static uint32_t frame_counter = 0;

#define AUTO_DROP_SPEED 5     // frames between automatic drops (faster!)
#define SOFT_DROP_SPEED 2      // frames between drops when holding down
#define MOVE_REPEAT_DELAY 10   // frames before auto-repeat starts

void tetris_controls_init(void) {
    // initialize arcade buttons
    arcade_init();
    last_buttons = arcade_read_all();
    last_move_time = 0;
    last_drop_time = 0;
    frame_counter = 0;
}

void tetris_controls_update(tetris_game_state_t* game) {
    frame_counter++;

    // read arcade buttons
    uint16_t buttons = arcade_read_all();
    uint16_t button_pressed = buttons & ~last_buttons;  // detect rising edge

    // SELECT button exits to menu (check this first, even if game over)
    if (button_pressed & (1 << ARCADE_BUTTON_SELECT)) {
        game->exitToMenu = true;
        last_buttons = buttons;
        return;
    }

    // If game over, don't process other controls
    if (game->game_over) {
        last_buttons = buttons;
        return;
    }

    // LEFT - move piece left
    if (button_pressed & (1 << ARCADE_BUTTON_LEFT)) {
        tetris_game_move_piece(game, 0, -1);
    }

    // RIGHT - move piece right
    if (button_pressed & (1 << ARCADE_BUTTON_RIGHT)) {
        tetris_game_move_piece(game, 0, 1);
    }

    // UP or A - rotate piece
    if (button_pressed & (1 << ARCADE_BUTTON_UP) ||
        button_pressed & (1 << ARCADE_BUTTON_A)) {
        tetris_game_rotate_piece(game);
    }

    // DOWN - soft drop (move down faster)
    bool soft_drop = buttons & (1 << ARCADE_BUTTON_DOWN);

    // B button - hard drop (instant drop)
    if (button_pressed & (1 << ARCADE_BUTTON_B)) {
        // Drop piece all the way down
        while (tetris_game_can_move(game, 1, 0)) {
            tetris_game_move_piece(game, 1, 0);
        }
        tetris_game_lock_piece(game);
        last_drop_time = frame_counter;
    }

    last_buttons = buttons;

    // UART input (keyboard controls)
    if (uart_available()) {
        char c = uart_getc();

        switch(c) {
            case 'a':
            case 'A':
                tetris_game_move_piece(game, 0, -1);
                break;
            case 'd':
            case 'D':
                tetris_game_move_piece(game, 0, 1);
                break;
            case 'w':
            case 'W':
            case ' ':  // space for rotate
                tetris_game_rotate_piece(game);
                break;
            case 's':
            case 'S':
                soft_drop = true;
                break;
            case '\r':  // Enter for hard drop
            case '\n':
                while (tetris_game_can_move(game, 1, 0)) {
                    tetris_game_move_piece(game, 1, 0);
                }
                tetris_game_lock_piece(game);
                last_drop_time = frame_counter;
                break;
        }
    }

    // Automatic/soft drop logic
    uint32_t drop_speed = soft_drop ? SOFT_DROP_SPEED : AUTO_DROP_SPEED;

    if (frame_counter - last_drop_time >= drop_speed) {
        if (tetris_game_can_move(game, 1, 0)) {
            tetris_game_move_piece(game, 1, 0);
        } else {
            // Piece can't move down, lock it
            tetris_game_lock_piece(game);
        }
        last_drop_time = frame_counter;
    }
}

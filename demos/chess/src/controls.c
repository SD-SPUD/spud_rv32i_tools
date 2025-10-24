#include "controls.h"

// previous button state for edge detection
static uint16_t last_buttons = 0;

// helper function to attempt picking up a piece
static void try_pickup_piece(cursor_t* cursor, game_state_t* game) {
    chess_piece_t piece = game_get_piece(game, cursor->row, cursor->col);

    // make sure we have a piece
    if (piece.type == PIECE_NONE) {
        return;
    }

    // check the color matches the current turn
    if ((game->turn == WHITE_TURN && piece.color != COLOR_WHITE_PIECE) ||
        (game->turn == BLACK_TURN && piece.color != COLOR_BLACK_PIECE)) {
        return;
    }

    cursor->is_holding_piece = 1;
    cursor->held_piece_row = cursor->row;
    cursor->held_piece_col = cursor->col;
    cursor->box_color = CURSOR_HOLDING_COLOR;
}

// helper function to attempt dropping a piece
static void try_drop_piece(cursor_t* cursor, game_state_t* game) {
    // validate move before dropping
    if (is_valid_move(cursor, game)) {
        // get the piece from the held position
        chess_piece_t piece = game_get_piece(game, cursor->held_piece_row, cursor->held_piece_col);

        // mark piece as moved
        piece.is_first_move = false;

        // move the piece
        game_set_piece(game, cursor->held_piece_row, cursor->held_piece_col, PIECE_NONE, COLOR_NONE);
        game->board[cursor->row][cursor->col] = piece;

        cursor->is_holding_piece = 0;
        cursor->box_color = CURSOR_COLOR;

        // switch turn after successful move
        game->turn = (game->turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN;
    }
    // if invalid, piece stays held and cursor stays in holding color
}

// helper function to handle select action (pickup or drop)
static void handle_select_action(cursor_t* cursor, game_state_t* game) {
    if (!cursor->is_holding_piece) {
        try_pickup_piece(cursor, game);
    } else {
        // check if cursor is on the same tile as the held piece
        if (cursor->row == cursor->held_piece_row && cursor->col == cursor->held_piece_col) {
            // cancel the selection
            cursor->is_holding_piece = 0;
            cursor->box_color = CURSOR_COLOR;
        } else {
            try_drop_piece(cursor, game);
        }
    }
}

// helper function to move cursor
static void move_cursor(cursor_t* cursor, int8_t row_delta, int8_t col_delta) {
    int8_t new_row = cursor->row + row_delta;
    int8_t new_col = cursor->col + col_delta;

    if (new_row >= 0 && new_row < 8) cursor->row = new_row;
    if (new_col >= 0 && new_col < 8) cursor->col = new_col;
}

void controls_init(cursor_t* cursor, spud_color_t box_color) {
    cursor->row = 0;
    cursor->col = 0;
    cursor->box_color = box_color;
    cursor->is_holding_piece = 0;
    cursor->held_piece_row = 0;
    cursor->held_piece_col = 0;

    // initialize arcade buttons
    arcade_init();

    last_buttons = arcade_read_all();
}

void controls_update(cursor_t* cursor, game_state_t* game) {
    // read arcade buttons
    uint16_t buttons = arcade_read_all();
    uint16_t button_pressed = buttons & ~last_buttons;  // detect rising edge

    // check for A button press (select/drop piece)
    if (button_pressed & (1 << ARCADE_BUTTON_A)) {
        handle_select_action(cursor, game);
    }

    // check for B button press (cancel selection)
    if (button_pressed & (1 << ARCADE_BUTTON_B)) {
        if (cursor->is_holding_piece) {
            cursor->is_holding_piece = 0;
            cursor->box_color = CURSOR_COLOR;
        }
    }

    // check for directional input from arcade buttons
    if (button_pressed & (1 << ARCADE_BUTTON_UP)) {
        move_cursor(cursor, -1, 0);
    }
    if (button_pressed & (1 << ARCADE_BUTTON_DOWN)) {
        move_cursor(cursor, 1, 0);
    }
    if (button_pressed & (1 << ARCADE_BUTTON_LEFT)) {
        move_cursor(cursor, 0, -1);
    }
    if (button_pressed & (1 << ARCADE_BUTTON_RIGHT)) {
        move_cursor(cursor, 0, 1);
    }

    last_buttons = buttons;

    // check for UART input (WASD + Enter)
    if (uart_available()) {
        char c = uart_getc();

        switch(c) {
            case 'w':
            case 'W':
                move_cursor(cursor, -1, 0);
                break;
            case 's':
            case 'S':
                move_cursor(cursor, 1, 0);
                break;
            case 'a':
            case 'A':
                move_cursor(cursor, 0, -1);
                break;
            case 'd':
            case 'D':
                move_cursor(cursor, 0, 1);
                break;
            case '\r':  // Enter key
            case '\n':  // Newline (some terminals send this)
                handle_select_action(cursor, game);
                break;
            case '\b':  // Backspace
            case 127:   // Delete (some terminals send this for backspace)
                if (cursor->is_holding_piece) {
                    cursor->is_holding_piece = 0;
                    cursor->box_color = CURSOR_COLOR;
                }
                break;
        }
    }
}

void controls_draw_cursor(cursor_t* cursor) {
    uint8_t x = cursor->col * SQUARE_SIZE;
    uint8_t y = cursor->row * SQUARE_SIZE;

    // draw 1-pixel thick box around the square
    // top edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x + i, y, cursor->box_color);
    }

    // bottom edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x + i, y + SQUARE_SIZE - 1, cursor->box_color);
    }

    // left edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x, y + i, cursor->box_color);
    }

    // right edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x + SQUARE_SIZE - 1, y + i, cursor->box_color);
    }
}

void controls_clear_cursor(cursor_t* cursor) {
    uint8_t x = cursor->col * SQUARE_SIZE;
    uint8_t y = cursor->row * SQUARE_SIZE;

    // determine square color for this position
    uint8_t is_light = (cursor->row + cursor->col) % 2;
    spud_color_t square_color = is_light ? LIGHT_SQUARE_COLOR : DARK_SQUARE_COLOR;

    // redraw the border with the square color to "clear" the cursor
    // top edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x + i, y, square_color);
    }

    // bottom edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x + i, y + SQUARE_SIZE - 1, square_color);
    }

    // left edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x, y + i, square_color);
    }

    // right edge
    for (uint8_t i = 0; i < SQUARE_SIZE; i++) {
        display_set_pixel(x + SQUARE_SIZE - 1, y + i, square_color);
    }
}

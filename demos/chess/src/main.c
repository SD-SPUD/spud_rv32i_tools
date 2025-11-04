#include "spudkit.h"
#include "board.h"
#include "game.h"
#include "controls.h"
#include "engine.h"

int main() {
    spudkit_init();
    uart_puts("welcome to chess\r\n");

    game_state_t game;
    game_init(&game);

    board_init();
    board_draw();
    game_draw_pieces(&game);
    display_update();
    uart_puts("chess game initialized\r\n");

    cursor_t white_cursor;
    controls_init(&white_cursor, CURSOR_COLOR);
    controls_draw_cursor(&white_cursor);

    cursor_t black_cursor;
    controls_init(&black_cursor, CURSOR_COLOR);
    controls_draw_cursor(&black_cursor);

    uart_puts("use WASD or arcade buttons to move cursor\r\n");
    uart_puts("press A button or Enter to select/drop pieces\r\n");
    uart_puts("press Y button to reset the game\r\n");

    uint16_t last_buttons = arcade_read_all();

    const int bignum = 1000000;
    int arr[bignum];

    while(1) {
        // check for Y button press (reset game)
        uint16_t buttons = arcade_read_all();
        uint16_t button_pressed = buttons & ~last_buttons;  // detect rising edge

        if (button_pressed & (1 << ARCADE_BUTTON_Y)) {
            // reset the game
            game_init(&game);

            // reset cursors
            white_cursor.row = 0;
            white_cursor.col = 0;
            white_cursor.is_holding_piece = 0;
            white_cursor.box_color = CURSOR_COLOR;

            black_cursor.row = 0;
            black_cursor.col = 0;
            black_cursor.is_holding_piece = 0;
            black_cursor.box_color = CURSOR_COLOR;

            uart_puts("game reset!\r\n");
        }

        last_buttons = buttons;

        board_draw();
        switch (game.turn) {
        case WHITE_TURN:
            controls_update(&white_cursor, &game);
            game_draw_pieces(&game);
            controls_draw_cursor(&white_cursor);
            break;
        case BLACK_TURN:
            controls_update(&black_cursor, &game);
            game_draw_pieces(&game);
            controls_draw_cursor(&black_cursor);
            break;
        }

        // draw eval bar if X button is held
        draw_eval_bar(&game);

        display_update();

        // small delay to prevent overwhelming the system
        for (volatile int i = 0; i < 1000; i++);
    }
    return 0;
}

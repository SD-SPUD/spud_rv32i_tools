#include "spudkit.h"
#include "board.h"
#include "game.h"
#include "controls.h"
#include "engine.h"

int chess_main() {
    uart_puts("welcome to chess\r\n");

    chess_game_state_t game;
    chess_game_init(&game);

    chess_board_init();
    chess_board_draw();
    chess_game_draw_pieces(&game);
    display_update();
    uart_puts("chess game initialized\r\n");

    chess_cursor_t white_cursor;
    chess_controls_init(&white_cursor, CURSOR_COLOR);
    chess_controls_draw_cursor(&white_cursor);

    chess_cursor_t black_cursor;
    chess_controls_init(&black_cursor, CURSOR_COLOR);
    chess_controls_draw_cursor(&black_cursor);

    uart_puts("use WASD or arcade buttons to move cursor\r\n");
    uart_puts("press A button or Enter to select/drop pieces\r\n");
    uart_puts("press Y button to reset the game\r\n");
    uart_puts("press SELECT button to exit to menu\r\n");

    uint16_t last_buttons = arcade_read_all();

    const int bignum = 1000000;
    int arr[bignum];

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
            chess_game_init(&game);

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

        chess_board_draw();
        switch (game.turn) {
        case WHITE_TURN:
            chess_controls_update(&white_cursor, &game);
            chess_game_draw_pieces(&game);
            chess_controls_draw_cursor(&white_cursor);
            break;
        case BLACK_TURN:
            chess_controls_update(&black_cursor, &game);
            chess_game_draw_pieces(&game);
            chess_controls_draw_cursor(&black_cursor);
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

int main() {
    spudkit_init();
    return chess_main();
}

#ifndef CHESS_CONTROLS_H
#define CHESS_CONTROLS_H

#include "spudkit.h"
#include "board.h"
#include "game.h"

// cursor state
typedef struct chess_cursor_t {
    uint8_t row;
    uint8_t col;
    spud_color_t box_color;
    uint8_t is_holding_piece;  // 1 if holding a piece, 0 otherwise
    uint8_t held_piece_row;    // original row of held piece
    uint8_t held_piece_col;    // original col of held piece
} chess_cursor_t;

// default cursor colors
#define CURSOR_COLOR COLOR_YELLOW
#define CURSOR_HOLDING_COLOR COLOR_GREEN

// control functions
void chess_controls_init(chess_cursor_t* cursor, spud_color_t box_color);
void chess_controls_update(chess_cursor_t* cursor, chess_game_state_t* game);
void chess_controls_draw_cursor(chess_cursor_t* cursor);
void chess_controls_clear_cursor(chess_cursor_t* cursor);

#endif // CHESS_CONTROLS_H

#ifndef CHESS_CONTROLS_H
#define CHESS_CONTROLS_H

#include "spudkit.h"
#include "board.h"
#include "game.h"

// cursor state
typedef struct cursor_t {
    uint8_t row;
    uint8_t col;
    spud_color_t box_color;
    uint8_t is_holding_piece;  // 1 if holding a piece, 0 otherwise
    uint8_t held_piece_row;    // original row of held piece
    uint8_t held_piece_col;    // original col of held piece
} cursor_t;

// default cursor colors
#define CURSOR_COLOR COLOR_YELLOW
#define CURSOR_HOLDING_COLOR COLOR_GREEN

// control functions
void controls_init(cursor_t* cursor, spud_color_t box_color);
void controls_update(cursor_t* cursor, game_state_t* game);
void controls_draw_cursor(cursor_t* cursor);
void controls_clear_cursor(cursor_t* cursor);

#endif // CHESS_CONTROLS_H

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <stdbool.h>
#include "spudkit.h"
#include "board.h"

// Forward declaration to avoid circular dependency
typedef struct cursor_t cursor_t;

typedef enum {
    WHITE_TURN,
    BLACK_TURN,
} turn_t;

// piece types
typedef enum {
    PIECE_NONE = 0,
    PIECE_PAWN,
    PIECE_ROOK,
    PIECE_KNIGHT,
    PIECE_BISHOP,
    PIECE_QUEEN,
    PIECE_KING
} piece_type_t;

// piece colors
typedef enum {
    COLOR_NONE = 0,
    COLOR_WHITE_PIECE = 1,
    COLOR_BLACK_PIECE = 2
} piece_color_t;

// combined piece representation
typedef struct {
    piece_type_t type;
    piece_color_t color;
    bool is_first_move;
} chess_piece_t;

// game state
typedef struct {
    turn_t turn;
    chess_piece_t board[8][8];  // [row][col]
} game_state_t;

// piece colors for drawing
#define WHITE_PIECE_COLOR COLOR_BLUE
#define BLACK_PIECE_COLOR COLOR_RED

// game functions
void game_init(game_state_t* game);
void game_draw_pieces(game_state_t* game);
void game_set_piece(game_state_t* game, uint8_t row, uint8_t col, piece_type_t type, piece_color_t color);
chess_piece_t game_get_piece(game_state_t* game, uint8_t row, uint8_t col);

// move validation
bool is_valid_move(cursor_t* cursor, game_state_t* game);

#endif // CHESS_GAME_H

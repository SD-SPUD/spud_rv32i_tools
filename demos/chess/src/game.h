#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <stdbool.h>
#include "spudkit.h"
#include "board.h"

// Forward declaration to avoid circular dependency
typedef struct chess_cursor_t chess_cursor_t;

typedef enum {
    WHITE_TURN,
    BLACK_TURN,
} chess_turn_t;

// piece types
typedef enum {
    CHESS_PIECE_NONE = 0,
    CHESS_PIECE_PAWN,
    CHESS_PIECE_ROOK,
    CHESS_PIECE_KNIGHT,
    CHESS_PIECE_BISHOP,
    CHESS_PIECE_QUEEN,
    CHESS_PIECE_KING
} chess_piece_type_t;

// piece colors
typedef enum {
    CHESS_COLOR_NONE = 0,
    CHESS_COLOR_WHITE_PIECE = 1,
    CHESS_COLOR_BLACK_PIECE = 2
} chess_piece_color_t;

// combined piece representation
typedef struct {
    chess_piece_type_t type;
    chess_piece_color_t color;
    bool is_first_move;
} chess_piece_t;

// game state
typedef struct {
    chess_turn_t turn;
    chess_piece_t board[8][8];  // [row][col]
    bool exitToMenu;  // exit to menu flag (set by SELECT button)
} chess_game_state_t;

// piece colors for drawing
#define WHITE_PIECE_COLOR COLOR_BLUE
#define BLACK_PIECE_COLOR COLOR_RED

// game functions
void chess_game_init(chess_game_state_t* game);
void chess_game_draw_pieces(chess_game_state_t* game);
void chess_game_set_piece(chess_game_state_t* game, uint8_t row, uint8_t col, chess_piece_type_t type, chess_piece_color_t color);
chess_piece_t chess_game_get_piece(chess_game_state_t* game, uint8_t row, uint8_t col);

// move validation
bool chess_is_valid_move(chess_cursor_t* cursor, chess_game_state_t* game);

#endif // CHESS_GAME_H

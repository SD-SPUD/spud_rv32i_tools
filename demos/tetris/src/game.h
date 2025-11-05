#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <stdbool.h>
#include "spudkit.h"
#include "board.h"

// Forward declaration to avoid circular dependency
typedef struct cursor_t cursor_t;

// piece types (7 standard tetris pieces)
typedef enum {
    PIECE_NONE = 0,
    PIECE_I,     // straight line (cyan)
    PIECE_O,     // square (yellow)
    PIECE_T,     // T-shape (purple)
    PIECE_S,     // S-shape (green)
    PIECE_Z,     // Z-shape (red)
    PIECE_J,     // J-shape (blue)
    PIECE_L      // L-shape (orange)
} piece_type_t;

// piece colors for drawing
#define PIECE_I_COLOR COLOR_CYAN
#define PIECE_O_COLOR COLOR_YELLOW
#define PIECE_T_COLOR COLOR_MAGENTA
#define PIECE_S_COLOR COLOR_GREEN
#define PIECE_Z_COLOR COLOR_RED
#define PIECE_J_COLOR COLOR_BLUE
#define PIECE_L_COLOR COLOR_WHITE

// piece rotation states (0, 90, 180, 270 degrees)
typedef enum {
    ROTATION_0 = 0,
    ROTATION_90 = 1,
    ROTATION_180 = 2,
    ROTATION_270 = 3
} rotation_t;

// single block in the tetris piece
typedef struct {
    piece_type_t type;
} tetris_block_t;

// active falling piece
typedef struct {
    piece_type_t type;
    rotation_t rotation;
    uint8_t row;  // position of piece anchor
    uint8_t col;  // position of piece anchor
} active_piece_t;

// game state
typedef struct {
    tetris_block_t board[BOARD_HEIGHT][BOARD_WIDTH];  // [row][col]
    active_piece_t active_piece;
    bool game_over;
    uint16_t score;
    uint16_t lines_cleared;
} game_state_t;

// game functions
void game_init(game_state_t* game);
void game_draw_board(game_state_t* game);
void game_draw_active_piece(game_state_t* game);
spud_color_t game_get_piece_color(piece_type_t type);

// piece manipulation
void game_spawn_piece(game_state_t* game);
bool game_can_move(game_state_t* game, int row_offset, int col_offset);
void game_move_piece(game_state_t* game, int row_offset, int col_offset);
bool game_can_rotate(game_state_t* game);
void game_rotate_piece(game_state_t* game);
void game_lock_piece(game_state_t* game);
void game_clear_lines(game_state_t* game);

// get piece shape data for rendering/collision
// returns array of 4 offsets from piece anchor
void game_get_piece_blocks(piece_type_t type, rotation_t rotation, int8_t offsets[4][2]);

#endif // TETRIS_GAME_H

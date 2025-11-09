#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <stdbool.h>
#include "spudkit.h"
#include "board.h"

// Forward declaration to avoid circular dependency
typedef struct tetris_cursor_t tetris_cursor_t;

// piece types (7 standard tetris pieces)
typedef enum {
    TETRIS_PIECE_NONE = 0,
    TETRIS_PIECE_I,     // straight line (cyan)
    TETRIS_PIECE_O,     // square (yellow)
    TETRIS_PIECE_T,     // T-shape (purple)
    TETRIS_PIECE_S,     // S-shape (green)
    TETRIS_PIECE_Z,     // Z-shape (red)
    TETRIS_PIECE_J,     // J-shape (blue)
    TETRIS_PIECE_L      // L-shape (orange)
} tetris_piece_type_t;

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
    TETRIS_ROTATION_0 = 0,
    TETRIS_ROTATION_90 = 1,
    TETRIS_ROTATION_180 = 2,
    TETRIS_ROTATION_270 = 3
} tetris_rotation_t;

// single block in the tetris piece
typedef struct {
    tetris_piece_type_t type;
} tetris_block_t;

// active falling piece
typedef struct {
    tetris_piece_type_t type;
    tetris_rotation_t rotation;
    uint8_t row;  // position of piece anchor
    uint8_t col;  // position of piece anchor
} tetris_active_piece_t;

// game state
typedef struct {
    tetris_block_t board[BOARD_HEIGHT][BOARD_WIDTH];  // [row][col]
    tetris_active_piece_t active_piece;
    bool game_over;
    bool exitToMenu;  // exit to menu flag (set by SELECT button)
    uint16_t score;
    uint16_t lines_cleared;
} tetris_game_state_t;

// game functions
void tetris_game_init(tetris_game_state_t* game);
void tetris_game_draw_board(tetris_game_state_t* game);
void tetris_game_draw_active_piece(tetris_game_state_t* game);
spud_color_t tetris_game_get_piece_color(tetris_piece_type_t type);

// piece manipulation
void tetris_game_spawn_piece(tetris_game_state_t* game);
bool tetris_game_can_move(tetris_game_state_t* game, int row_offset, int col_offset);
void tetris_game_move_piece(tetris_game_state_t* game, int row_offset, int col_offset);
bool tetris_game_can_rotate(tetris_game_state_t* game);
void tetris_game_rotate_piece(tetris_game_state_t* game);
void tetris_game_lock_piece(tetris_game_state_t* game);
void tetris_game_clear_lines(tetris_game_state_t* game);

// get piece shape data for rendering/collision
// returns array of 4 offsets from piece anchor
void tetris_game_get_piece_blocks(tetris_piece_type_t type, tetris_rotation_t rotation, int8_t offsets[4][2]);

// draw score display on the right side
void tetris_game_draw_score(tetris_game_state_t* game);

#endif // TETRIS_GAME_H

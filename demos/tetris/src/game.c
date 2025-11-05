#include "game.h"
#include "controls.h"
#include <stddef.h>

// Tetris piece definitions
// Each piece has 4 rotation states
// Pieces are defined as offsets from an anchor point [row_offset, col_offset]
// Standard tetris rotation system (SRS)

// I piece (straight line) - 4x1 or 1x4
static const int8_t piece_I[4][4][2] = {
    // Rotation 0 (horizontal)
    {{0, -1}, {0, 0}, {0, 1}, {0, 2}},
    // Rotation 90 (vertical)
    {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
    // Rotation 180 (horizontal)
    {{0, -1}, {0, 0}, {0, 1}, {0, 2}},
    // Rotation 270 (vertical)
    {{-1, 0}, {0, 0}, {1, 0}, {2, 0}}
};

// O piece (square) - 2x2 (doesn't rotate)
static const int8_t piece_O[4][4][2] = {
    // All rotations are the same
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}}
};

// T piece
static const int8_t piece_T[4][4][2] = {
    // Rotation 0 (T pointing up)
    {{0, -1}, {0, 0}, {0, 1}, {1, 0}},
    // Rotation 90 (T pointing right)
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
    // Rotation 180 (T pointing down)
    {{0, -1}, {0, 0}, {0, 1}, {-1, 0}},
    // Rotation 270 (T pointing left)
    {{-1, 0}, {0, 0}, {1, 0}, {0, -1}}
};

// S piece (green)
static const int8_t piece_S[4][4][2] = {
    // Rotation 0
    {{0, -1}, {0, 0}, {1, 0}, {1, 1}},
    // Rotation 90
    {{-1, 0}, {0, 0}, {0, -1}, {1, -1}},
    // Rotation 180
    {{0, -1}, {0, 0}, {1, 0}, {1, 1}},
    // Rotation 270
    {{-1, 0}, {0, 0}, {0, -1}, {1, -1}}
};

// Z piece (red)
static const int8_t piece_Z[4][4][2] = {
    // Rotation 0
    {{0, 0}, {0, 1}, {1, -1}, {1, 0}},
    // Rotation 90
    {{-1, -1}, {0, -1}, {0, 0}, {1, 0}},
    // Rotation 180
    {{0, 0}, {0, 1}, {1, -1}, {1, 0}},
    // Rotation 270
    {{-1, -1}, {0, -1}, {0, 0}, {1, 0}}
};

// J piece (blue)
static const int8_t piece_J[4][4][2] = {
    // Rotation 0
    {{0, -1}, {0, 0}, {0, 1}, {1, 1}},
    // Rotation 90
    {{-1, 0}, {0, 0}, {1, 0}, {1, -1}},
    // Rotation 180
    {{0, -1}, {0, 0}, {0, 1}, {-1, -1}},
    // Rotation 270
    {{-1, 0}, {0, 0}, {1, 0}, {-1, 1}}
};

// L piece (orange)
static const int8_t piece_L[4][4][2] = {
    // Rotation 0
    {{0, -1}, {0, 0}, {0, 1}, {1, -1}},
    // Rotation 90
    {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
    // Rotation 180
    {{0, -1}, {0, 0}, {0, 1}, {-1, 1}},
    // Rotation 270
    {{-1, 0}, {0, 0}, {1, 0}, {-1, -1}}
};

// Get piece blocks based on type and rotation
void game_get_piece_blocks(piece_type_t type, rotation_t rotation, int8_t offsets[4][2]) {
    const int8_t (*piece_data)[4][2] = NULL;

    switch(type) {
        case PIECE_I: piece_data = piece_I; break;
        case PIECE_O: piece_data = piece_O; break;
        case PIECE_T: piece_data = piece_T; break;
        case PIECE_S: piece_data = piece_S; break;
        case PIECE_Z: piece_data = piece_Z; break;
        case PIECE_J: piece_data = piece_J; break;
        case PIECE_L: piece_data = piece_L; break;
        default: return;
    }

    for (int i = 0; i < 4; i++) {
        offsets[i][0] = piece_data[rotation][i][0];
        offsets[i][1] = piece_data[rotation][i][1];
    }
}

// Get color for piece type
spud_color_t game_get_piece_color(piece_type_t type) {
    switch(type) {
        case PIECE_I: return PIECE_I_COLOR;
        case PIECE_O: return PIECE_O_COLOR;
        case PIECE_T: return PIECE_T_COLOR;
        case PIECE_S: return PIECE_S_COLOR;
        case PIECE_Z: return PIECE_Z_COLOR;
        case PIECE_J: return PIECE_J_COLOR;
        case PIECE_L: return PIECE_L_COLOR;
        default: return COLOR_BLACK;
    }
}

// Initialize game
void game_init(game_state_t* game) {
    // Clear the board
    for (uint8_t row = 0; row < BOARD_HEIGHT; row++) {
        for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
            game->board[row][col].type = PIECE_NONE;
        }
    }

    game->game_over = false;
    game->score = 0;
    game->lines_cleared = 0;

    // Spawn first piece
    game_spawn_piece(game);
}

// Spawn a new piece at the top
void game_spawn_piece(game_state_t* game) {
    // Simple random piece selection (cycles through pieces)
    static uint8_t next_piece = PIECE_I;

    game->active_piece.type = next_piece;
    game->active_piece.rotation = ROTATION_0;
    game->active_piece.row = 1;  // Start near top
    game->active_piece.col = BOARD_WIDTH / 2;  // Center

    // Cycle to next piece
    next_piece++;
    if (next_piece > PIECE_L) {
        next_piece = PIECE_I;
    }

    // Check if piece can spawn (game over check)
    if (!game_can_move(game, 0, 0)) {
        game->game_over = true;
    }
}

// Check if piece can move to a new position
bool game_can_move(game_state_t* game, int row_offset, int col_offset) {
    int8_t offsets[4][2];
    game_get_piece_blocks(game->active_piece.type, game->active_piece.rotation, offsets);

    for (int i = 0; i < 4; i++) {
        int new_row = game->active_piece.row + offsets[i][0] + row_offset;
        int new_col = game->active_piece.col + offsets[i][1] + col_offset;

        // Check boundaries
        if (new_row < 0 || new_row >= BOARD_HEIGHT ||
            new_col < 0 || new_col >= BOARD_WIDTH) {
            return false;
        }

        // Check collision with placed blocks
        if (game->board[new_row][new_col].type != PIECE_NONE) {
            return false;
        }
    }

    return true;
}

// Move piece
void game_move_piece(game_state_t* game, int row_offset, int col_offset) {
    if (game_can_move(game, row_offset, col_offset)) {
        game->active_piece.row += row_offset;
        game->active_piece.col += col_offset;
    }
}

// Check if piece can rotate
bool game_can_rotate(game_state_t* game) {
    rotation_t new_rotation = (game->active_piece.rotation + 1) % 4;
    int8_t offsets[4][2];
    game_get_piece_blocks(game->active_piece.type, new_rotation, offsets);

    for (int i = 0; i < 4; i++) {
        int new_row = game->active_piece.row + offsets[i][0];
        int new_col = game->active_piece.col + offsets[i][1];

        // Check boundaries
        if (new_row < 0 || new_row >= BOARD_HEIGHT ||
            new_col < 0 || new_col >= BOARD_WIDTH) {
            return false;
        }

        // Check collision with placed blocks
        if (game->board[new_row][new_col].type != PIECE_NONE) {
            return false;
        }
    }

    return true;
}

// Rotate piece
void game_rotate_piece(game_state_t* game) {
    if (game_can_rotate(game)) {
        game->active_piece.rotation = (game->active_piece.rotation + 1) % 4;
    }
}

// Lock piece in place
void game_lock_piece(game_state_t* game) {
    int8_t offsets[4][2];
    game_get_piece_blocks(game->active_piece.type, game->active_piece.rotation, offsets);

    for (int i = 0; i < 4; i++) {
        int row = game->active_piece.row + offsets[i][0];
        int col = game->active_piece.col + offsets[i][1];

        if (row >= 0 && row < BOARD_HEIGHT && col >= 0 && col < BOARD_WIDTH) {
            game->board[row][col].type = game->active_piece.type;
        }
    }

    // Check for line clears
    game_clear_lines(game);

    // Spawn new piece
    game_spawn_piece(game);
}

// Clear completed lines
void game_clear_lines(game_state_t* game) {
    uint8_t lines_cleared_now = 0;

    // Check each row from bottom to top
    for (int row = BOARD_HEIGHT - 1; row >= 0; row--) {
        bool line_full = true;

        // Check if row is full
        for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
            if (game->board[row][col].type == PIECE_NONE) {
                line_full = false;
                break;
            }
        }

        if (line_full) {
            lines_cleared_now++;

            // Move all rows above down
            for (int r = row; r > 0; r--) {
                for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
                    game->board[r][col] = game->board[r-1][col];
                }
            }

            // Clear top row
            for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
                game->board[0][col].type = PIECE_NONE;
            }

            // Check this row again (since we moved rows down)
            row++;
        }
    }

    // Update score
    if (lines_cleared_now > 0) {
        game->lines_cleared += lines_cleared_now;
        // Simple scoring: 100 points per line
        game->score += lines_cleared_now * 100;
    }
}

// Draw the placed blocks on the board
void game_draw_board(game_state_t* game) {
    for (uint8_t row = 0; row < BOARD_HEIGHT; row++) {
        for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
            if (game->board[row][col].type != PIECE_NONE) {
                spud_color_t color = game_get_piece_color(game->board[row][col].type);
                uint8_t x = col * BLOCK_SIZE;
                uint8_t y = row * BLOCK_SIZE;
                display_fill_rect(x, y, BLOCK_SIZE, BLOCK_SIZE, color);
            }
        }
    }
}

// Draw the active falling piece
void game_draw_active_piece(game_state_t* game) {
    if (game->game_over) return;

    int8_t offsets[4][2];
    game_get_piece_blocks(game->active_piece.type, game->active_piece.rotation, offsets);
    spud_color_t color = game_get_piece_color(game->active_piece.type);

    for (int i = 0; i < 4; i++) {
        int row = game->active_piece.row + offsets[i][0];
        int col = game->active_piece.col + offsets[i][1];

        if (row >= 0 && row < BOARD_HEIGHT && col >= 0 && col < BOARD_WIDTH) {
            uint8_t x = col * BLOCK_SIZE;
            uint8_t y = row * BLOCK_SIZE;
            display_fill_rect(x, y, BLOCK_SIZE, BLOCK_SIZE, color);
        }
    }
}

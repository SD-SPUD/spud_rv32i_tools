#include "game.h"
#include "controls.h"
#include <stddef.h>

// 8x8 pixel patterns for chess pieces
// 1 = piece pixel, 0 = transparent (use square color)

// Pawn pattern
static const uint8_t pawn_pattern[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0}
};

// Rook pattern
static const uint8_t rook_pattern[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,1,0,1,1,0,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

// Knight pattern
static const uint8_t knight_pattern[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,0,0},
    {0,0,0,1,1,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

// Bishop pattern
static const uint8_t bishop_pattern[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

// Queen pattern
static const uint8_t queen_pattern[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

// King pattern
static const uint8_t king_pattern[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,1,0,1,1,0,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

// helper function to get pattern for piece type
static const uint8_t (*get_piece_pattern(piece_type_t type))[8] {
    switch(type) {
        case PIECE_PAWN:   return pawn_pattern;
        case PIECE_ROOK:   return rook_pattern;
        case PIECE_KNIGHT: return knight_pattern;
        case PIECE_BISHOP: return bishop_pattern;
        case PIECE_QUEEN:  return queen_pattern;
        case PIECE_KING:   return king_pattern;
        default:           return NULL;
    }
}

void game_init(game_state_t* game) {
    // initialize turn - white goes first
    game->turn = WHITE_TURN;

    // clear the board
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            game->board[row][col].type = PIECE_NONE;
            game->board[row][col].color = COLOR_NONE;
            game->board[row][col].is_first_move = true;
        }
    }

    // set up initial chess position
    // black pieces (top, row 0-1)
    game->board[0][0] = (chess_piece_t){PIECE_ROOK, COLOR_BLACK_PIECE, true};
    game->board[0][1] = (chess_piece_t){PIECE_KNIGHT, COLOR_BLACK_PIECE, true};
    game->board[0][2] = (chess_piece_t){PIECE_BISHOP, COLOR_BLACK_PIECE, true};
    game->board[0][3] = (chess_piece_t){PIECE_QUEEN, COLOR_BLACK_PIECE, true};
    game->board[0][4] = (chess_piece_t){PIECE_KING, COLOR_BLACK_PIECE, true};
    game->board[0][5] = (chess_piece_t){PIECE_BISHOP, COLOR_BLACK_PIECE, true};
    game->board[0][6] = (chess_piece_t){PIECE_KNIGHT, COLOR_BLACK_PIECE, true};
    game->board[0][7] = (chess_piece_t){PIECE_ROOK, COLOR_BLACK_PIECE, true};

    // black pawns
    for (uint8_t col = 0; col < 8; col++) {
        game->board[1][col] = (chess_piece_t){PIECE_PAWN, COLOR_BLACK_PIECE, true};
    }

    // white pawns
    for (uint8_t col = 0; col < 8; col++) {
        game->board[6][col] = (chess_piece_t){PIECE_PAWN, COLOR_WHITE_PIECE, true};
    }

    // white pieces (bottom, row 7)
    game->board[7][0] = (chess_piece_t){PIECE_ROOK, COLOR_WHITE_PIECE, true};
    game->board[7][1] = (chess_piece_t){PIECE_KNIGHT, COLOR_WHITE_PIECE, true};
    game->board[7][2] = (chess_piece_t){PIECE_BISHOP, COLOR_WHITE_PIECE, true};
    game->board[7][3] = (chess_piece_t){PIECE_QUEEN, COLOR_WHITE_PIECE, true};
    game->board[7][4] = (chess_piece_t){PIECE_KING, COLOR_WHITE_PIECE, true};
    game->board[7][5] = (chess_piece_t){PIECE_BISHOP, COLOR_WHITE_PIECE, true};
    game->board[7][6] = (chess_piece_t){PIECE_KNIGHT, COLOR_WHITE_PIECE, true};
    game->board[7][7] = (chess_piece_t){PIECE_ROOK, COLOR_WHITE_PIECE, true};
}

void game_draw_pieces(game_state_t* game) {
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            chess_piece_t piece = game->board[row][col];

            if (piece.type != PIECE_NONE) {
                const uint8_t (*pattern)[8] = get_piece_pattern(piece.type);
                if (pattern) {
                    // determine piece color
                    spud_color_t piece_color = (piece.color == COLOR_WHITE_PIECE) ?
                                               WHITE_PIECE_COLOR : BLACK_PIECE_COLOR;

                    // draw the piece pattern
                    for (uint8_t py = 0; py < 8; py++) {
                        for (uint8_t px = 0; px < 8; px++) {
                            if (pattern[py][px]) {
                                uint8_t screen_x = col * SQUARE_SIZE + px;
                                uint8_t screen_y = row * SQUARE_SIZE + py;
                                display_set_pixel(screen_x, screen_y, piece_color);
                            }
                        }
                    }
                }
            }
        }
    }
}

void game_set_piece(game_state_t* game, uint8_t row, uint8_t col, piece_type_t type, piece_color_t color) {
    if (row < 8 && col < 8) {
        game->board[row][col].type = type;
        game->board[row][col].color = color;
    }
}

chess_piece_t game_get_piece(game_state_t* game, uint8_t row, uint8_t col) {
    chess_piece_t empty = {PIECE_NONE, COLOR_NONE, false};
    if (row < 8 && col < 8) {
        return game->board[row][col];
    }
    return empty;
}

// check if the piece is valid on cursor
bool is_valid_move(cursor_t* cursor, game_state_t* game){
    chess_piece_t* piece = &game->board[cursor->held_piece_row][cursor->held_piece_col];

    int row_diff = cursor->row - cursor->held_piece_row;
    int col_diff = cursor->col - cursor->held_piece_col;

    if(col_diff == 0 && row_diff == 0) return false;

    // white moves up (negative row_diff), black moves down (positive row_diff)
    int direction = (piece->color == COLOR_WHITE_PIECE) ? -1 : 1;


    // TODO: check if move put king in check

    // check each piece
    switch (piece->type) {
    case PIECE_PAWN:
    {
        // check if moving in correct direction
        if (row_diff * direction <= 0) return false;

        // moving straight forward
        if (col_diff == 0) {
            // destination must be empty
            if (game->board[cursor->row][cursor->col].type != PIECE_NONE) return false;

            // can move 1 or 2 squares on first move, otherwise only 1
            int max_move = piece->is_first_move ? 2 : 1;
            if (row_diff * direction > max_move) return false;
        }
        // capturing diagonally
        else if (col_diff == 1 || col_diff == -1) {
            // can only move 1 square diagonally
            if (row_diff * direction != 1) return false;

            // must capture an enemy piece
            chess_piece_t target = game->board[cursor->row][cursor->col];
            if (target.type == PIECE_NONE || target.color == piece->color) return false;
        } else {
            return false;
        }
    }
    break;
    case PIECE_ROOK:
    {
        // rook moves horizontally or vertically
        if (row_diff != 0 && col_diff != 0) return false;
        if (row_diff == 0 && col_diff == 0) return false;

        // check for pieces blocking the path
        int step_row = (row_diff == 0) ? 0 : (row_diff > 0 ? 1 : -1);
        int step_col = (col_diff == 0) ? 0 : (col_diff > 0 ? 1 : -1);

        int check_row = cursor->held_piece_row + step_row;
        int check_col = cursor->held_piece_col + step_col;

        // check all squares between start and destination
        while (check_row != cursor->row || check_col != cursor->col) {
            if (game->board[check_row][check_col].type != PIECE_NONE) return false;
            check_row += step_row;
            check_col += step_col;
        }

        // destination must be empty or contain enemy piece
        chess_piece_t target = game->board[cursor->row][cursor->col];
        if (target.type != PIECE_NONE && target.color == piece->color) return false;
    }
    break;
    case PIECE_KNIGHT:
    {
        // knight moves in L shape: 2 squares in one direction, 1 in perpendicular
        int abs_row = (row_diff > 0) ? row_diff : -row_diff;
        int abs_col = (col_diff > 0) ? col_diff : -col_diff;

        // valid knight moves: (2,1) or (1,2)
        if (!((abs_row == 2 && abs_col == 1) || (abs_row == 1 && abs_col == 2))) return false;

        // destination must be empty or contain enemy piece
        chess_piece_t target = game->board[cursor->row][cursor->col];
        if (target.type != PIECE_NONE && target.color == piece->color) return false;
    }
    break;
    case PIECE_BISHOP:
    {
        // bishop moves diagonally
        int abs_row = (row_diff > 0) ? row_diff : -row_diff;
        int abs_col = (col_diff > 0) ? col_diff : -col_diff;

        // must move diagonally (same distance in both directions)
        if (abs_row != abs_col) return false;

        // check for pieces blocking the path
        int step_row = (row_diff > 0) ? 1 : -1;
        int step_col = (col_diff > 0) ? 1 : -1;

        int check_row = cursor->held_piece_row + step_row;
        int check_col = cursor->held_piece_col + step_col;

        // check all squares between start and destination
        while (check_row != cursor->row || check_col != cursor->col) {
            if (game->board[check_row][check_col].type != PIECE_NONE) return false;
            check_row += step_row;
            check_col += step_col;
        }

        // destination must be empty or contain enemy piece
        chess_piece_t target = game->board[cursor->row][cursor->col];
        if (target.type != PIECE_NONE && target.color == piece->color) return false;
    }
    break;
    case PIECE_QUEEN:
    {
        // queen moves like rook or bishop (horizontally, vertically, or diagonally)
        int abs_row = (row_diff > 0) ? row_diff : -row_diff;
        int abs_col = (col_diff > 0) ? col_diff : -col_diff;

        // must move either straight or diagonally
        if (row_diff != 0 && col_diff != 0 && abs_row != abs_col) return false;

        // check for pieces blocking the path
        int step_row = (row_diff == 0) ? 0 : (row_diff > 0 ? 1 : -1);
        int step_col = (col_diff == 0) ? 0 : (col_diff > 0 ? 1 : -1);

        int check_row = cursor->held_piece_row + step_row;
        int check_col = cursor->held_piece_col + step_col;

        // check all squares between start and destination
        while (check_row != cursor->row || check_col != cursor->col) {
            if (game->board[check_row][check_col].type != PIECE_NONE) return false;
            check_row += step_row;
            check_col += step_col;
        }

        // destination must be empty or contain enemy piece
        chess_piece_t target = game->board[cursor->row][cursor->col];
        if (target.type != PIECE_NONE && target.color == piece->color) return false;
    }
    break;
    case PIECE_KING:
    {
        // king moves one square in any direction
        int abs_row = (row_diff > 0) ? row_diff : -row_diff;
        int abs_col = (col_diff > 0) ? col_diff : -col_diff;

        // can only move 1 square
        if (abs_row > 1 || abs_col > 1) return false;

        // destination must be empty or contain enemy piece
        chess_piece_t target = game->board[cursor->row][cursor->col];
        if (target.type != PIECE_NONE && target.color == piece->color) return false;
    }
    break;
    case PIECE_NONE:
        // no piece to move
        return false;

    }

    return true;
}

#include "engine.h"
#include "controls.h"

// helper function to get piece value
static int get_piece_value(piece_type_t type) {
    switch (type) {
        case PIECE_PAWN:   return PAWN_VALUE;
        case PIECE_KNIGHT: return KNIGHT_VALUE;
        case PIECE_BISHOP: return BISHOP_VALUE;
        case PIECE_ROOK:   return ROOK_VALUE;
        case PIECE_QUEEN:  return QUEEN_VALUE;
        case PIECE_KING:   return KING_VALUE;
        default:           return 0;
    }
}

// helper function to get piece-square table value
static int get_piece_square_value(piece_type_t type, uint8_t row, uint8_t col, piece_color_t color) {
    // for black pieces, mirror the row (flip vertically)
    uint8_t table_row = (color == COLOR_BLACK_PIECE) ? (7 - row) : row;

    switch (type) {
        case PIECE_PAWN:   return pawn_table[table_row][col];
        case PIECE_KNIGHT: return knight_table[table_row][col];
        case PIECE_BISHOP: return bishop_table[table_row][col];
        case PIECE_ROOK:   return rook_table[table_row][col];
        case PIECE_QUEEN:  return queen_table[table_row][col];
        case PIECE_KING:   return king_table[table_row][col];
        default:           return 0;
    }
}

// positive is for white and neg is for black
int evaluate_position(game_state_t* game) {
    int score = 0;

    // iterate through all squares
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            chess_piece_t piece = game->board[row][col];

            if (piece.type == PIECE_NONE) {
                continue;
            }

            // get material value
            int piece_value = get_piece_value(piece.type);

            // get positional value from piece-square tables
            int positional_value = get_piece_square_value(piece.type, row, col, piece.color);

            // total value for this piece
            int total_value = piece_value + positional_value;

            // add to score (positive for white, negative for black)
            if (piece.color == COLOR_WHITE_PIECE) {
                score += total_value;
            } else {
                score -= total_value;
            }
        }
    }

    return score;
}

// draw eval bar on the left side of screen (only if X button is held)
void draw_eval_bar(game_state_t* game) {
    // check if X button is being held (active low)
    uint16_t buttons = arcade_read_all();
    uint8_t x_button_held = (buttons & (1 << ARCADE_BUTTON_X)) == 0;

    // only draw if button is held
    if (!x_button_held) {
        return;
    }

    // evaluate the position
    int eval_score = evaluate_position(game);

    // clamp score to max range
    if (eval_score > EVAL_BAR_MAX_SCORE) eval_score = EVAL_BAR_MAX_SCORE;
    if (eval_score < -EVAL_BAR_MAX_SCORE) eval_score = -EVAL_BAR_MAX_SCORE;

    // calculate percentage (0-100)
    // score of 0 = 50% white, 50% black (split in middle)
    // positive score = more white, less black
    // negative score = less white, more black
    int white_percent = 50 + (eval_score * 50) / EVAL_BAR_MAX_SCORE;

    // calculate pixel heights (64 pixels tall)
    uint8_t white_height = (white_percent * 64) / 100;
    uint8_t black_height = 64 - white_height;

    // draw black portion (top)
    for (uint8_t y = 0; y < black_height; y++) {
        for (uint8_t x = EVAL_BAR_X; x < EVAL_BAR_X + EVAL_BAR_WIDTH; x++) {
            display_set_pixel(x, y, COLOR_BLACK);
        }
    }

    // draw white portion (bottom)
    for (uint8_t y = black_height; y < 64; y++) {
        for (uint8_t x = EVAL_BAR_X; x < EVAL_BAR_X + EVAL_BAR_WIDTH; x++) {
            display_set_pixel(x, y, COLOR_WHITE);
        }
    }
}

// helper function to add possible moves for a piece
static int add_moves_for_piece(game_state_t* game_states, game_state_t* game_base, uint8_t row, uint8_t col, int num_moves, const int8_t moves[][2], int move_count, bool sliding) {
    cursor_t temp_cursor;
    temp_cursor.held_piece_row = row;
    temp_cursor.held_piece_col = col;
    temp_cursor.is_holding_piece = 1;

    for (int i = 0; i < move_count; i++) {
        int8_t row_delta = moves[i][0];
        int8_t col_delta = moves[i][1];

        // for sliding pieces (rook, bishop, queen), keep moving in direction until blocked
        int distance = sliding ? 8 : 1;

        for (int d = 1; d < distance; d++) {
            int8_t dest_row = row + (row_delta * d);
            int8_t dest_col = col + (col_delta * d);

            // check bounds
            if (dest_row < 0 || dest_row >= 8 || dest_col < 0 || dest_col >= 8) {
                break;
            }

            temp_cursor.row = dest_row;
            temp_cursor.col = dest_col;

            if (is_valid_move(&temp_cursor, game_base)) {
                // copy the base game state
                game_states[num_moves] = *game_base;

                // get the piece (mark as moved)
                chess_piece_t moving_piece = game_base->board[row][col];
                moving_piece.is_first_move = false;

                // make the move
                game_states[num_moves].board[row][col].type = PIECE_NONE;
                game_states[num_moves].board[row][col].color = COLOR_NONE;
                game_states[num_moves].board[dest_row][dest_col] = moving_piece;

                // switch turn
                game_states[num_moves].turn = (game_base->turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN;

                num_moves++;

                // if piece captured or pawn move, stop sliding
                if (game_base->board[dest_row][dest_col].type != PIECE_NONE) {
                    break;
                }
            } else if (sliding) {
                // if move is blocked, stop sliding in this direction
                break;
            }
        }
    }
    return num_moves;
}

// get all possible board states for a piece at the given position
// returns the number of possible moves found
int get_possible_board_states(game_state_t* game_states, game_state_t* game_base, uint8_t row, uint8_t col) {
    int num_moves = 0;

    // get the piece at the specified position
    chess_piece_t piece = game_base->board[row][col];

    // if there's no piece, return 0
    if (piece.type == PIECE_NONE) {
        return 0;
    }

    // define move patterns for each piece type
    switch (piece.type) {
        case PIECE_PAWN: {
            // pawn moves depend on color
            int8_t direction = (piece.color == COLOR_WHITE_PIECE) ? -1 : 1;

            // forward moves
            static const int8_t pawn_forward[][2] = {{-1, 0}, {-2, 0}};  // will multiply by direction
            int8_t forward_moves[2][2] = {{direction, 0}, {direction * 2, 0}};
            num_moves = add_moves_for_piece(game_states, game_base, row, col, num_moves, forward_moves, 2, false);

            // diagonal captures
            int8_t diagonal_moves[2][2] = {{direction, -1}, {direction, 1}};
            num_moves = add_moves_for_piece(game_states, game_base, row, col, num_moves, diagonal_moves, 2, false);
            break;
        }
        case PIECE_KNIGHT: {
            static const int8_t knight_moves[][2] = {
                {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                {1, -2}, {1, 2}, {2, -1}, {2, 1}
            };
            num_moves = add_moves_for_piece(game_states, game_base, row, col, num_moves, knight_moves, 8, false);
            break;
        }
        case PIECE_BISHOP: {
            static const int8_t bishop_moves[][2] = {
                {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
            };
            num_moves = add_moves_for_piece(game_states, game_base, row, col, num_moves, bishop_moves, 4, true);
            break;
        }
        case PIECE_ROOK: {
            static const int8_t rook_moves[][2] = {
                {-1, 0}, {1, 0}, {0, -1}, {0, 1}
            };
            num_moves = add_moves_for_piece(game_states, game_base, row, col, num_moves, rook_moves, 4, true);
            break;
        }
        case PIECE_QUEEN: {
            static const int8_t queen_moves[][2] = {
                {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                {0, 1}, {1, -1}, {1, 0}, {1, 1}
            };
            num_moves = add_moves_for_piece(game_states, game_base, row, col, num_moves, queen_moves, 8, true);
            break;
        }
        case PIECE_KING: {
            static const int8_t king_moves[][2] = {
                {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                {0, 1}, {1, -1}, {1, 0}, {1, 1}
            };
            num_moves = add_moves_for_piece(game_states, game_base, row, col, num_moves, king_moves, 8, false);
            break;
        }
        default:
            break;
    }

    return num_moves;
}

// my ai bot
int minimax(int depth, turn_t turn, game_state_t game) {
    if(depth > MAX_DEPTH) {
        return evaluate_position(&game);
    }
    // can be better
    // iterate through all squares
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            chess_piece_t piece = game.board[row][col];
            if(turn == WHITE_TURN) {
                int val = INT_INF;

            }
            else {
                int val = INT_NEG_INF;
            }

         }
    }

}

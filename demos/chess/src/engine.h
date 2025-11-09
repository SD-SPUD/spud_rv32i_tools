#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include "game.h"

// https://www.chessprogramming.org/Simplified_Evaluation_Function
// piece-square tables for white pieces (values are from white's perspective)
// for black pieces, use mirrored values (flip the board vertically)
// indexed as [row][col] to match game board structure


#define MAX_DEPTH 10
#define INT_INF (int)0xFFFFFFF
#define INT_NEG_INF (int)-0xFFFFFFF

// pawn piece-square table
static const int pawn_table[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    { 50, 50, 50, 50, 50, 50, 50, 50 },
    { 10, 10, 20, 30, 30, 20, 10, 10 },
    {  5,  5, 10, 25, 25, 10,  5,  5 },
    {  0,  0,  0, 20, 20,  0,  0,  0 },
    {  5, -5,-10,  0,  0,-10, -5,  5 },
    {  5, 10, 10,-20,-20, 10, 10,  5 },
    {  0,  0,  0,  0,  0,  0,  0,  0 }
};

// knight piece-square table
static const int knight_table[8][8] = {
    { -50,-40,-30,-30,-30,-30,-40,-50 },
    { -40,-20,  0,  0,  0,  0,-20,-40 },
    { -30,  0, 10, 15, 15, 10,  0,-30 },
    { -30,  5, 15, 20, 20, 15,  5,-30 },
    { -30,  0, 15, 20, 20, 15,  0,-30 },
    { -30,  5, 10, 15, 15, 10,  5,-30 },
    { -40,-20,  0,  5,  5,  0,-20,-40 },
    { -50,-40,-30,-30,-30,-30,-40,-50 }
};

// bishop piece-square table
static const int bishop_table[8][8] = {
    { -20,-10,-10,-10,-10,-10,-10,-20 },
    { -10,  0,  0,  0,  0,  0,  0,-10 },
    { -10,  0,  5, 10, 10,  5,  0,-10 },
    { -10,  5,  5, 10, 10,  5,  5,-10 },
    { -10,  0, 10, 10, 10, 10,  0,-10 },
    { -10, 10, 10, 10, 10, 10, 10,-10 },
    { -10,  5,  0,  0,  0,  0,  5,-10 },
    { -20,-10,-10,-10,-10,-10,-10,-20 }
};

// rook piece-square table
static const int rook_table[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    {  5, 10, 10, 10, 10, 10, 10,  5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    {  0,  0,  0,  5,  5,  0,  0,  0 }
};

// queen piece-square table
static const int queen_table[8][8] = {
    { -20,-10,-10, -5, -5,-10,-10,-20 },
    { -10,  0,  0,  0,  0,  0,  0,-10 },
    { -10,  0,  5,  5,  5,  5,  0,-10 },
    {  -5,  0,  5,  5,  5,  5,  0, -5 },
    {   0,  0,  5,  5,  5,  5,  0, -5 },
    { -10,  5,  5,  5,  5,  5,  0,-10 },
    { -10,  0,  5,  0,  0,  0,  0,-10 },
    { -20,-10,-10, -5, -5,-10,-10,-20 }
};

// king piece-square table
static const int king_table[8][8] = {
    { -30,-40,-40,-50,-50,-40,-40,-30 },
    { -30,-40,-40,-50,-50,-40,-40,-30 },
    { -30,-40,-40,-50,-50,-40,-40,-30 },
    { -30,-40,-40,-50,-50,-40,-40,-30 },
    { -20,-30,-30,-40,-40,-30,-30,-20 },
    { -10,-20,-20,-20,-20,-20,-20,-10 },
    {  20, 20,  0,  0,  0,  0, 20, 20 },
    {  20, 30, 10,  0,  0, 10, 30, 20 }
};

// piece base values
#define PAWN_VALUE 100
#define KNIGHT_VALUE 320
#define BISHOP_VALUE 330
#define ROOK_VALUE 500
#define QUEEN_VALUE 900
#define KING_VALUE 20000

// eval bar settings
#define EVAL_BAR_WIDTH 4
#define EVAL_BAR_X 0
#define EVAL_BAR_MAX_SCORE 2000  // score at which bar is fully one color

// evaluation function
int chess_evaluate_position(chess_game_state_t* game);

// draw eval bar on the left side of screen (only if X button is held)
void draw_eval_bar(chess_game_state_t* game);

// get all possible board states for a piece at the given position
// returns the number of possible moves found
int chess_get_possible_board_states(chess_game_state_t* game_states, chess_game_state_t* game_base, uint8_t row, uint8_t col);

#endif // CHESS_ENGINE_H

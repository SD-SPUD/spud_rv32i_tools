#ifndef TETRIS_CONTROLS_H
#define TETRIS_CONTROLS_H

#include "spudkit.h"
#include "board.h"
#include "game.h"

// control functions
void tetris_controls_init(void);
void tetris_controls_update(tetris_game_state_t* game);

#endif // TETRIS_CONTROLS_H

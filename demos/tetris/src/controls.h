#ifndef TETRIS_CONTROLS_H
#define TETRIS_CONTROLS_H

#include "spudkit.h"
#include "board.h"
#include "game.h"

// control functions
void controls_init(void);
void controls_update(game_state_t* game);

#endif // TETRIS_CONTROLS_H

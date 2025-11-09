#ifndef CONTROLS_H
#define CONTROLS_H

#include "spudkit.h"
#include "game.h"

// Initialize controls
void spudman_controls_init(void);

// Update controls and handle input
void spudman_controls_update(game_state_t* game);

#endif // CONTROLS_H

#ifndef CONTROLS_H
#define CONTROLS_H

#include "spudkit.h"
#include "game.h"

// Initialize controls
void controls_init(void);

// Update controls and handle input (both arcade buttons and UART keyboard)
void controls_update(game_state_t* game);

#endif // CONTROLS_H

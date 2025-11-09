#ifndef TRACK_H
#define TRACK_H

#include "spudkit.h"
#include "game.h"

// Initialize track rendering
void track_init(void);

// Draw the road/track with scrolling effect
void track_draw(game_state_t* game);

// Draw landmarks (UCF buildings, palm trees, etc.)
void track_draw_landmarks(game_state_t* game);

#endif // TRACK_H

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <stdbool.h>
#include "spudkit.h"

#define GRID_WIDTH 16
#define GRID_HEIGHT 16
#define BLOCK_SIZE 4
#define MAX_TAIL_LENGTH 100

// direction enum
typedef enum {
    DIR_STOP = 0,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
} direction_t;

// game state
typedef struct {
    int x;              // head x position
    int y;              // head y position
    int foodX;          // food x position
    int foodY;          // food y position
    int score;          // current score
    int tailX[MAX_TAIL_LENGTH];  // tail x positions
    int tailY[MAX_TAIL_LENGTH];  // tail y positions
    int nTail;          // number of tail segments
    direction_t dir;    // current direction
    bool gameOver;      // game over flag
    bool exitToMenu;    // exit to menu flag (set by SELECT button)
} game_state_t;

// game functions
void snake_game_init(game_state_t* game);
void snake_game_update(game_state_t* game);
void snake_game_draw(game_state_t* game);
void snake_game_spawn_food(game_state_t* game);

#endif // SNAKE_GAME_H

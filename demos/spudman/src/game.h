#ifndef GAME_H
#define GAME_H

#include "spudkit.h"

// Game constants
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 64
#define MAZE_WIDTH 16
#define MAZE_HEIGHT 16
#define TILE_SIZE 4

// Tile types
#define TILE_EMPTY 0
#define TILE_WALL 1
#define TILE_PELLET 2
#define TILE_POWER_PELLET 3
#define TILE_SPUDMAN_START 4
#define TILE_GHOST_START 5

// Directions
#define DIR_NONE 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4

// Ghost states
#define GHOST_STATE_CHASE 0
#define GHOST_STATE_SCARED 1
#define GHOST_STATE_EATEN 2

// Game constants
#define MAX_GHOSTS 4
#define POWER_PELLET_DURATION 300  // frames (~5 seconds)
#define STARTING_LIVES 3
#define GHOST_POINTS 200
#define PELLET_POINTS 10
#define POWER_PELLET_POINTS 50

// Colors
#define COLOR_WALL 0x0000FF  // Blue walls
#define COLOR_PELLET 0x00FF00  // Green sprout pellets
#define COLOR_POWER_PELLET 0x00FF00  // Larger green sprout
#define COLOR_SPUDMAN 0x804000  // Brown potato
#define COLOR_SPUDMAN_EYE 0xFFFFFF  // White eyes
#define COLOR_GHOST_RED 0xFF0000
#define COLOR_GHOST_PINK 0xFFB6C1
#define COLOR_GHOST_CYAN 0x00FFFF
#define COLOR_GHOST_ORANGE 0xFFA500
#define COLOR_GHOST_SCARED 0x0000FF  // Blue when scared
#define COLOR_BACKGROUND 0x000000  // Black

// Spudman state
typedef struct {
    int8_t x;  // Grid position
    int8_t y;
    int8_t pixel_x;  // Pixel position for smooth movement
    int8_t pixel_y;
    uint8_t direction;
    uint8_t next_direction;  // Queued direction
} spudman_t;

// Ghost state
typedef struct {
    int8_t x;
    int8_t y;
    int8_t pixel_x;
    int8_t pixel_y;
    uint8_t direction;
    uint8_t state;  // chase, scared, or eaten
    spud_color_t color;
} ghost_t;

// Game state
typedef struct {
    uint8_t maze[MAZE_HEIGHT][MAZE_WIDTH];
    spudman_t spudman;
    ghost_t ghosts[MAX_GHOSTS];
    uint16_t score;
    uint8_t lives;
    uint8_t pellets_remaining;
    uint16_t power_timer;  // Countdown for power pellet effect
    uint32_t frame_count;
    uint8_t game_over;
} game_state_t;

// Function prototypes
void game_init(game_state_t* game);
void game_update(game_state_t* game);
void game_draw(game_state_t* game);
void game_check_collisions(game_state_t* game);
uint8_t game_can_move(game_state_t* game, int8_t x, int8_t y);

#endif // GAME_H

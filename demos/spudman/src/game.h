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

// Colors (RGB888 format: 0xBBGGRR for hardware, BGR byte order)
#define COLOR_WALL 0xFF0000  // Blue walls (0x0000FF in RGB -> 0xFF0000 in BGR)
#define COLOR_PELLET 0x00FF00  // Green sprout pellets
#define COLOR_POWER_PELLET 0x00FF00  // Larger green sprout
#define COLOR_SPUDMAN 0x004080  // Brown potato (0x804000 RGB -> 0x004080 BGR)
#define COLOR_SPUDMAN_EYE 0xFFFFFF  // White eyes
#define COLOR_GHOST_RED 0x0000FF  // Red (0xFF0000 RGB -> 0x0000FF BGR)
#define COLOR_GHOST_PINK 0xC1B6FF  // Pink (0xFFB6C1 RGB -> 0xC1B6FF BGR)
#define COLOR_GHOST_CYAN 0xFFFF00  // Cyan (0x00FFFF RGB -> 0xFFFF00 BGR)
#define COLOR_GHOST_ORANGE 0x00A5FF  // Orange (0xFFA500 RGB -> 0x00A5FF BGR)
#define COLOR_GHOST_SCARED 0xFF0000  // Blue when scared (0x0000FF RGB -> 0xFF0000 BGR)
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

// Game states
#define GAME_STATE_HOME 0
#define GAME_STATE_PLAYING 1
#define GAME_STATE_GAME_OVER 2

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
    uint8_t game_state;  // HOME, PLAYING, or GAME_OVER
} game_state_t;

// Function prototypes
void game_init(game_state_t* game);
void game_reset(game_state_t* game);  // Reset game but keep in playing state
void game_update(game_state_t* game);
void game_draw(game_state_t* game);
void game_draw_home_screen(void);
void game_check_collisions(game_state_t* game);
uint8_t game_can_move(game_state_t* game, int8_t x, int8_t y);

#endif // GAME_H

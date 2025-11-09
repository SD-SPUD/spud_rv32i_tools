#ifndef GAME_H
#define GAME_H

#include "spudkit.h"

// Game constants
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 64
#define NUM_LANES 3
#define LANE_WIDTH 16
#define ROAD_WIDTH (NUM_LANES * LANE_WIDTH)
#define ROAD_START_X ((SCREEN_WIDTH - ROAD_WIDTH) / 2)

// Player car constants
#define PLAYER_CAR_WIDTH 10
#define PLAYER_CAR_HEIGHT 14
#define PLAYER_Y 45  // Near bottom of screen
#define PLAYER_START_LANE 1  // Middle lane

// Game speed and timing
#define BASE_SCROLL_SPEED 2
#define BOOST_DURATION 30  // frames
#define COLLISION_SLOWDOWN 60  // frames
#define SLOWDOWN_SPEED 1

// Bot car constants
#define MAX_BOT_CARS 4
#define BOT_CAR_WIDTH 10
#define BOT_CAR_HEIGHT 12
#define BOT_SPAWN_INTERVAL 40  // frames between spawns (faster spawning)

// Landmark constants
#define MAX_LANDMARKS 8

// Landmark types
#define LANDMARK_NONE 0
#define LANDMARK_POND 1
#define LANDMARK_HEC 2
#define LANDMARK_FLAG 3
#define LANDMARK_PALM_LEFT 4
#define LANDMARK_PALM_RIGHT 5
#define LANDMARK_CONE 6

// Speed Racer inspired colors
#define COLOR_ROAD_GRAY 0x404040
#define COLOR_GRASS_GREEN 0x00AA00
#define COLOR_LANE_WHITE 0xFFFFFF
#define COLOR_LANE_YELLOW 0xFFFF00
#define COLOR_PLAYER_CAR 0xFFFFFF  // White Mach 5
#define COLOR_PLAYER_ACCENT 0xFF0000  // Red accents
#define COLOR_POTATO_BROWN 0x804000
#define COLOR_WATER_BLUE 0x0066FF
#define COLOR_FOUNTAIN_WHITE 0xFFFFFF
#define COLOR_BUILDING_GRAY 0x808080
#define COLOR_PALM_TRUNK 0x804000
#define COLOR_PALM_LEAVES 0x00FF00
#define COLOR_FLAG_RED 0xFF0000
#define COLOR_FLAG_BLUE 0x0000FF
#define COLOR_CONE_ORANGE 0xFF8800

// Player state
typedef struct {
    int8_t lane;  // 0, 1, or 2
    int8_t target_lane;
    int8_t x;
    int8_t y;
    uint8_t boost_timer;
    uint8_t collision_timer;
    uint32_t distance;  // Score
    uint32_t frame_count;
    uint32_t elapsed_seconds;  // Time in seconds
} player_t;

// Bot car state
typedef struct {
    int8_t lane;
    int16_t y;  // Can be negative when spawning
    uint8_t active;
    spud_color_t color;
} bot_car_t;

// Landmark state
typedef struct {
    uint8_t type;
    int16_t y;  // Scroll position
    uint8_t active;
} landmark_t;

// Game state
typedef struct {
    player_t player;
    bot_car_t bot_cars[MAX_BOT_CARS];
    landmark_t landmarks[MAX_LANDMARKS];
    uint8_t spawn_timer;
} game_state_t;

// Initialize game state
void game_init(game_state_t* game);

// Update game logic
void game_update(game_state_t* game);

// Draw game objects (player, bots)
void game_draw(game_state_t* game);

// Helper functions
int8_t game_get_lane_x(int8_t lane);
void game_spawn_bot_car(game_state_t* game);
void game_spawn_landmark(game_state_t* game);
void game_check_collisions(game_state_t* game);

#endif // GAME_H

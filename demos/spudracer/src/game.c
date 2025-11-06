#include "game.h"

// Player car sprite - Mach 5 style with visible potato driver
static const uint8_t player_car_sprite[PLAYER_CAR_HEIGHT][PLAYER_CAR_WIDTH] = {
    {0,0,0,1,1,1,1,0,0,0},  // Top - spoiler
    {0,0,1,1,1,1,1,1,0,0},  // Hood
    {0,1,1,2,2,2,2,1,1,0},  // Windshield with potato
    {0,1,2,3,3,3,3,2,1,0},  // Potato driver visible
    {0,1,2,3,3,3,3,2,1,0},  // More potato
    {0,1,1,2,2,2,2,1,1,0},  // Windshield bottom
    {1,1,1,1,1,1,1,1,1,1},  // Body
    {1,4,1,1,1,1,1,1,4,1},  // Body with red accents
    {1,4,1,1,1,1,1,1,4,1},  // More red accents
    {1,1,1,1,1,1,1,1,1,1},  // Body
    {1,1,1,1,1,1,1,1,1,1},  // Lower body
    {0,1,1,1,1,1,1,1,1,0},  // Wheel wells
    {0,5,5,1,1,1,1,5,5,0},  // Wheels
    {0,0,0,1,1,1,1,0,0,0},  // Exhaust
};

// Bot car sprite (simple)
static const uint8_t bot_car_sprite[BOT_CAR_HEIGHT][BOT_CAR_WIDTH] = {
    {0,0,0,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {0,1,2,1,1,1,1,2,1,0},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,0},
    {0,3,3,1,1,1,1,3,3,0},
    {0,0,0,1,1,1,1,0,0,0},
};

void game_init(game_state_t* game) {
    // Initialize player
    game->player.lane = PLAYER_START_LANE;
    game->player.target_lane = PLAYER_START_LANE;
    game->player.x = game_get_lane_x(PLAYER_START_LANE);
    game->player.y = PLAYER_Y;
    game->player.boost_timer = 0;
    game->player.collision_timer = 0;
    game->player.distance = 0;
    game->player.frame_count = 0;
    game->player.elapsed_seconds = 0;

    // Initialize bot cars
    for (int i = 0; i < MAX_BOT_CARS; i++) {
        game->bot_cars[i].active = 0;
    }

    // Initialize landmarks
    for (int i = 0; i < MAX_LANDMARKS; i++) {
        game->landmarks[i].active = 0;
    }

    game->spawn_timer = 0;
}

int8_t game_get_lane_x(int8_t lane) {
    return ROAD_START_X + (lane * LANE_WIDTH) + (LANE_WIDTH - PLAYER_CAR_WIDTH) / 2;
}

void game_spawn_bot_car(game_state_t* game) {
    // Find inactive bot car slot
    for (int i = 0; i < MAX_BOT_CARS; i++) {
        if (!game->bot_cars[i].active) {
            game->bot_cars[i].active = 1;
            game->bot_cars[i].lane = game->player.frame_count % NUM_LANES;  // Pseudo-random lane
            game->bot_cars[i].y = SCREEN_HEIGHT / 3;  // Start at horizon

            // Vary bot car colors
            spud_color_t colors[] = {0xFF0000, 0x0000FF, 0xFFFF00, 0xFF00FF};
            game->bot_cars[i].color = colors[i % 4];
            uart_puts("Bot car spawned!\r\n");
            break;
        }
    }
}

void game_spawn_landmark(game_state_t* game) {
    // Find inactive landmark slot
    for (int i = 0; i < MAX_LANDMARKS; i++) {
        if (!game->landmarks[i].active) {
            game->landmarks[i].active = 1;
            game->landmarks[i].y = -20;

            // Cycle through landmark types
            uint8_t type_cycle = (game->player.frame_count / 200) % 7;
            if (type_cycle == 0) game->landmarks[i].type = LANDMARK_POND;
            else if (type_cycle == 1) game->landmarks[i].type = LANDMARK_HEC;
            else if (type_cycle == 2) game->landmarks[i].type = LANDMARK_FLAG;
            else if (type_cycle == 3) game->landmarks[i].type = LANDMARK_PALM_LEFT;
            else if (type_cycle == 4) game->landmarks[i].type = LANDMARK_PALM_RIGHT;
            else if (type_cycle == 5) game->landmarks[i].type = LANDMARK_CONE;
            else game->landmarks[i].type = LANDMARK_PALM_LEFT;
            break;
        }
    }
}

void game_check_collisions(game_state_t* game) {
    // Skip collision if boosting
    if (game->player.boost_timer > 0 || game->player.collision_timer > 0) {
        return;
    }

    // Check collision with bot cars
    for (int i = 0; i < MAX_BOT_CARS; i++) {
        if (game->bot_cars[i].active && game->bot_cars[i].lane == game->player.lane) {
            // Simple bounding box collision
            int bot_y_top = game->bot_cars[i].y;
            int bot_y_bottom = game->bot_cars[i].y + BOT_CAR_HEIGHT;
            int player_y_top = game->player.y;
            int player_y_bottom = game->player.y + PLAYER_CAR_HEIGHT;

            if (!(bot_y_bottom < player_y_top || bot_y_top > player_y_bottom)) {
                // Collision detected!
                game->player.collision_timer = COLLISION_SLOWDOWN;
                game->bot_cars[i].active = 0;  // Remove bot car
                uart_puts("CRASH! Slowdown!\r\n");
            }
        }
    }
}

void game_update(game_state_t* game) {
    // Update player lane position (smooth movement)
    int8_t target_x = game_get_lane_x(game->player.target_lane);
    if (game->player.x < target_x) {
        game->player.x += 2;
        if (game->player.x > target_x) game->player.x = target_x;
    } else if (game->player.x > target_x) {
        game->player.x -= 2;
        if (game->player.x < target_x) game->player.x = target_x;
    }

    // Update current lane when reached target
    if (game->player.x == target_x) {
        game->player.lane = game->player.target_lane;
    }

    // Update timers
    if (game->player.boost_timer > 0) game->player.boost_timer--;
    if (game->player.collision_timer > 0) game->player.collision_timer--;

    // Update bot cars
    uint8_t scroll_speed = (game->player.collision_timer > 0) ? SLOWDOWN_SPEED : BASE_SCROLL_SPEED;
    for (int i = 0; i < MAX_BOT_CARS; i++) {
        if (game->bot_cars[i].active) {
            game->bot_cars[i].y += scroll_speed;

            // Deactivate if off screen
            if (game->bot_cars[i].y > SCREEN_HEIGHT) {
                game->bot_cars[i].active = 0;
            }
        }
    }

    // Update landmarks
    for (int i = 0; i < MAX_LANDMARKS; i++) {
        if (game->landmarks[i].active) {
            game->landmarks[i].y += scroll_speed;

            // Deactivate if off screen
            if (game->landmarks[i].y > SCREEN_HEIGHT + 20) {
                game->landmarks[i].active = 0;
            }
        }
    }

    // Spawn new bot cars
    game->spawn_timer++;
    if (game->spawn_timer >= BOT_SPAWN_INTERVAL) {
        game_spawn_bot_car(game);
        game->spawn_timer = 0;
    }

    // Spawn landmarks periodically
    if (game->player.frame_count % 150 == 0) {
        game_spawn_landmark(game);
    }

    // Check collisions
    game_check_collisions(game);

    // Update score (distance)
    if (game->player.collision_timer == 0) {
        game->player.distance += BASE_SCROLL_SPEED;
    } else {
        game->player.distance += SLOWDOWN_SPEED;
    }

    game->player.frame_count++;

    // Update elapsed time (assuming ~60 fps, so 60 frames = 1 second)
    if (game->player.frame_count % 60 == 0) {
        game->player.elapsed_seconds++;
    }
}

static void draw_player_car(game_state_t* game) {
    int horizon_y = SCREEN_HEIGHT / 3;

    // Calculate player's X position based on current lane and perspective
    ffloat player_depth = FFDIV(FFLOAT(game->player.y - horizon_y), FFLOAT(SCREEN_HEIGHT - horizon_y));
    int road_width_at_player = FFINT(FFLOAT(8) + FFMULT(player_depth, FFLOAT(48)));
    int road_left_at_player = (SCREEN_WIDTH - road_width_at_player) / 2;
    int lane_width_at_player = road_width_at_player / NUM_LANES;
    int player_x = road_left_at_player + (game->player.lane * lane_width_at_player) + (lane_width_at_player - PLAYER_CAR_WIDTH) / 2;

    spud_color_t colors[6] = {
        COLOR_BLACK,           // 0
        COLOR_PLAYER_CAR,      // 1 - white body
        COLOR_CYAN,            // 2 - windshield
        COLOR_POTATO_BROWN,    // 3 - potato driver
        COLOR_PLAYER_ACCENT,   // 4 - red accents
        COLOR_BLACK            // 5 - wheels
    };

    // Boost effect - flash white/yellow
    if (game->player.boost_timer > 0) {
        colors[1] = (game->player.boost_timer % 4 < 2) ? COLOR_YELLOW : COLOR_WHITE;
    }

    for (int sy = 0; sy < PLAYER_CAR_HEIGHT; sy++) {
        for (int sx = 0; sx < PLAYER_CAR_WIDTH; sx++) {
            uint8_t pixel = player_car_sprite[sy][sx];
            if (pixel > 0) {
                int px = player_x + sx;
                int py = game->player.y + sy;
                if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                    display_set_pixel(px, py, colors[pixel]);
                }
            }
        }
    }
}

static void draw_bot_car(bot_car_t* bot) {
    if (!bot->active) return;

    int horizon_y = SCREEN_HEIGHT / 3;

    // Skip if above horizon
    if (bot->y < horizon_y) return;

    // Calculate scale based on distance from horizon (perspective)
    ffloat depth = FFDIV(FFLOAT(bot->y - horizon_y), FFLOAT(SCREEN_HEIGHT - horizon_y));
    int scale = FFINT(FFLOAT(3) + FFMULT(depth, FFLOAT(7)));  // Scale from 3 to 10

    // Calculate road width at bot's Y position for lane positioning
    int road_width = FFINT(FFLOAT(8) + FFMULT(depth, FFLOAT(48)));
    int road_left = (SCREEN_WIDTH - road_width) / 2;

    // Calculate bot X position based on lane within the perspective road
    int lane_width = road_width / NUM_LANES;
    int car_x = road_left + (bot->lane * lane_width) + (lane_width - scale) / 2;

    spud_color_t colors[4] = {
        COLOR_BLACK,     // 0
        bot->color,      // 1 - car body
        COLOR_CYAN,      // 2 - windshield
        COLOR_BLACK      // 3 - wheels
    };

    // Draw simplified bot car (just a colored rectangle for now)
    for (int sy = 0; sy < scale; sy++) {
        for (int sx = 0; sx < scale; sx++) {
            int px = car_x + sx;
            int py = bot->y + sy;
            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                display_set_pixel(px, py, bot->color);
            }
        }
    }
}

void game_draw(game_state_t* game) {
    // Draw bot cars
    for (int i = 0; i < MAX_BOT_CARS; i++) {
        draw_bot_car(&game->bot_cars[i]);
    }

    // Draw player car
    draw_player_car(game);

    // Draw score (top left)
    char score_text[16];
    uint32_t score = game->player.distance / 10;

    // Simple integer to string (no sprintf on bare metal)
    score_text[0] = 'D';
    score_text[1] = ':';
    score_text[2] = '0' + ((score / 1000) % 10);
    score_text[3] = '0' + ((score / 100) % 10);
    score_text[4] = '0' + ((score / 10) % 10);
    score_text[5] = '0' + (score % 10);
    score_text[6] = '\0';

    display_draw_string(1, 1, score_text, COLOR_WHITE, COLOR_BLACK);

    // Draw timer (top center)
    char timer_text[16];
    uint32_t minutes = game->player.elapsed_seconds / 60;
    uint32_t seconds = game->player.elapsed_seconds % 60;

    timer_text[0] = '0' + (minutes / 10);
    timer_text[1] = '0' + (minutes % 10);
    timer_text[2] = ':';
    timer_text[3] = '0' + (seconds / 10);
    timer_text[4] = '0' + (seconds % 10);
    timer_text[5] = '\0';

    display_draw_string(20, 1, timer_text, COLOR_YELLOW, COLOR_BLACK);

    // Draw collision warning
    if (game->player.collision_timer > 0) {
        display_draw_string(20, 1, "HIT!", COLOR_RED, COLOR_BLACK);
    }

    // Draw boost indicator
    if (game->player.boost_timer > 0) {
        display_draw_string(45, 1, "GO!", COLOR_YELLOW, COLOR_BLACK);
    }
}

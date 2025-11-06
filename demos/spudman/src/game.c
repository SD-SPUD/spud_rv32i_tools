#include "game.h"

// UCF-themed maze (16x16 grid, 0=empty+pellet, 1=wall, 3=power pellet, 5=ghost home)
// All 0s changed to 1s - blank canvas for you to customize!
// Change 1s to 0s to create paths, use 3 for power pellets, use 5 for ghost home
static const uint8_t initial_maze[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,3,0,0,0,0,0,0,0,0,0,0,0,0,3,1},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    {1,0,1,1,0,1,0,0,0,0,1,0,0,0,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,0,0,1,0,1}000
    {1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1},
    {1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,1},  // Ghost home in center
    {1,0,0,0,0,1,0,0,0,0,0,0,1,1,0,1},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,5,5,0,1,0,1,1,0,1},
    {1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1},
    {1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1}
    
    // {1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    // {1,3,0,0,0,0,0,0,0,0,0,0,0,0,3,1},
    // {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    // {1,0,1,1,0,1,0,0,0,0,1,0,0,0,0,1},
    // {1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1},
    // {1,0,1,1,0,1,0,1,1,1,1,0,0,1,0,1},
    // {1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1},
    // {1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,1},  // Ghost home in center
    // {1,0,0,0,0,1,0,0,0,0,0,0,1,1,0,1},
    // {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    // {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    // {1,0,1,1,0,1,0,5,5,0,1,0,1,1,0,1},
    // {1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1},
    // {1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1},
    // {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    // {1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1}
};

// Spudman sprite (4x4, tiny potato with eyes)
static const uint8_t spudman_sprite[TILE_SIZE][TILE_SIZE] = {
    {0,1,1,0},
    {1,2,2,1},
    {1,1,1,1},
    {0,1,1,0}
};

// Ghost sprite (4x4, simplified ghost)
static const uint8_t ghost_sprite[TILE_SIZE][TILE_SIZE] = {
    {0,1,1,0},
    {1,2,2,1},
    {1,1,1,1},
    {1,0,1,0}
};

void game_init(game_state_t* game) {
    // Copy initial maze
    game->pellets_remaining = 0;
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            game->maze[y][x] = initial_maze[y][x];
            // Convert empty spaces to pellets (except special tiles)
            if (game->maze[y][x] == TILE_EMPTY) {
                game->maze[y][x] = TILE_PELLET;
                game->pellets_remaining++;
            } else if (game->maze[y][x] == TILE_POWER_PELLET) {
                game->pellets_remaining++;
            }
        }
    }

    // Initialize Spudman at starting position
    game->spudman.x = 1;
    game->spudman.y = 13;
    game->spudman.pixel_x = game->spudman.x * TILE_SIZE;
    game->spudman.pixel_y = game->spudman.y * TILE_SIZE;
    game->spudman.direction = DIR_NONE;
    game->spudman.next_direction = DIR_NONE;

    // Initialize ghosts
    spud_color_t ghost_colors[MAX_GHOSTS] = {
        COLOR_GHOST_RED,
        COLOR_GHOST_PINK,
        COLOR_GHOST_CYAN,
        COLOR_GHOST_ORANGE
    };

    for (int i = 0; i < MAX_GHOSTS; i++) {
        game->ghosts[i].x = 7 + (i % 2);
        game->ghosts[i].y = 11 + (i / 2);
        game->ghosts[i].pixel_x = game->ghosts[i].x * TILE_SIZE;
        game->ghosts[i].pixel_y = game->ghosts[i].y * TILE_SIZE;
        game->ghosts[i].direction = DIR_UP;
        game->ghosts[i].state = GHOST_STATE_CHASE;
        game->ghosts[i].color = ghost_colors[i];
    }

    // Initialize game state
    game->score = 0;
    game->lives = STARTING_LIVES;
    game->power_timer = 0;
    game->frame_count = 0;
    game->game_over = 0;
}

uint8_t game_can_move(game_state_t* game, int8_t x, int8_t y) {
    if (x < 0 || x >= MAZE_WIDTH || y < 0 || y >= MAZE_HEIGHT) {
        return 0;
    }
    return game->maze[y][x] != TILE_WALL;
}

void game_update_spudman(game_state_t* game) {
    spudman_t* sp = &game->spudman;

    // Try to change direction if queued
    if (sp->next_direction != DIR_NONE) {
        int8_t next_x = sp->x;
        int8_t next_y = sp->y;

        switch (sp->next_direction) {
            case DIR_UP: next_y--; break;
            case DIR_DOWN: next_y++; break;
            case DIR_LEFT: next_x--; break;
            case DIR_RIGHT: next_x++; break;
        }

        if (game_can_move(game, next_x, next_y)) {
            sp->direction = sp->next_direction;
            sp->next_direction = DIR_NONE;
        }
    }

    // Move spudman
    if (sp->direction != DIR_NONE) {
        int8_t new_x = sp->x;
        int8_t new_y = sp->y;

        switch (sp->direction) {
            case DIR_UP: new_y--; break;
            case DIR_DOWN: new_y++; break;
            case DIR_LEFT: new_x--; break;
            case DIR_RIGHT: new_x++; break;
        }

        // Wraparound tunnels (left/right and top/bottom)
        if (new_x < 0) {
            new_x = MAZE_WIDTH - 1;
        } else if (new_x >= MAZE_WIDTH) {
            new_x = 0;
        }

        if (new_y < 0) {
            new_y = MAZE_HEIGHT - 1;
        } else if (new_y >= MAZE_HEIGHT) {
            new_y = 0;
        }

        if (game_can_move(game, new_x, new_y)) {
            sp->x = new_x;
            sp->y = new_y;
            sp->pixel_x = sp->x * TILE_SIZE;
            sp->pixel_y = sp->y * TILE_SIZE;

            // Collect pellet
            uint8_t tile = game->maze[sp->y][sp->x];
            if (tile == TILE_PELLET) {
                game->maze[sp->y][sp->x] = TILE_EMPTY;
                game->score += PELLET_POINTS;
                game->pellets_remaining--;
            } else if (tile == TILE_POWER_PELLET) {
                game->maze[sp->y][sp->x] = TILE_EMPTY;
                game->score += POWER_PELLET_POINTS;
                game->pellets_remaining--;
                game->power_timer = POWER_PELLET_DURATION;

                // Make all ghosts scared
                for (int i = 0; i < MAX_GHOSTS; i++) {
                    if (game->ghosts[i].state != GHOST_STATE_EATEN) {
                        game->ghosts[i].state = GHOST_STATE_SCARED;
                    }
                }
            }
        }
    }
}

void game_update_ghosts(game_state_t* game) {
    // Simple ghost AI: move towards spudman (only update every 3 frames for faster movement)
    if (game->frame_count % 3 != 0) {
        return;
    }

    for (int i = 0; i < MAX_GHOSTS; i++) {
        ghost_t* ghost = &game->ghosts[i];

        // Scared ghosts move away from spudman
        int8_t target_x = game->spudman.x;
        int8_t target_y = game->spudman.y;

        if (ghost->state == GHOST_STATE_SCARED) {
            // Move away from spudman
            target_x = 2 * ghost->x - target_x;
            target_y = 2 * ghost->y - target_y;
        }

        // Try to move closer to target
        int8_t dx = target_x - ghost->x;
        int8_t dy = target_y - ghost->y;

        // Choose direction based on largest delta
        uint8_t directions[4] = {DIR_NONE, DIR_NONE, DIR_NONE, DIR_NONE};
        int count = 0;

        if (dx > 0 && game_can_move(game, ghost->x + 1, ghost->y)) directions[count++] = DIR_RIGHT;
        if (dx < 0 && game_can_move(game, ghost->x - 1, ghost->y)) directions[count++] = DIR_LEFT;
        if (dy > 0 && game_can_move(game, ghost->x, ghost->y + 1)) directions[count++] = DIR_DOWN;
        if (dy < 0 && game_can_move(game, ghost->x, ghost->y - 1)) directions[count++] = DIR_UP;

        // Pick random valid direction if any
        if (count > 0) {
            ghost->direction = directions[(game->frame_count + i) % count];

            // Move ghost
            switch (ghost->direction) {
                case DIR_UP: ghost->y--; break;
                case DIR_DOWN: ghost->y++; break;
                case DIR_LEFT: ghost->x--; break;
                case DIR_RIGHT: ghost->x++; break;
            }

            // Wraparound tunnels (left/right)
            if (ghost->x < 0) {
                ghost->x = MAZE_WIDTH - 1;
            } else if (ghost->x >= MAZE_WIDTH) {
                ghost->x = 0;
            }

            ghost->pixel_x = ghost->x * TILE_SIZE;
            ghost->pixel_y = ghost->y * TILE_SIZE;
        }
    }
}

void game_check_collisions(game_state_t* game) {
    for (int i = 0; i < MAX_GHOSTS; i++) {
        ghost_t* ghost = &game->ghosts[i];

        if (ghost->x == game->spudman.x && ghost->y == game->spudman.y) {
            if (ghost->state == GHOST_STATE_SCARED) {
                // Eat the ghost!
                game->score += GHOST_POINTS;
                // Respawn ghost at home and return to chase mode
                ghost->x = 7 + (i % 2);
                ghost->y = 8 + (i / 2);
                ghost->pixel_x = ghost->x * TILE_SIZE;
                ghost->pixel_y = ghost->y * TILE_SIZE;
                ghost->state = GHOST_STATE_CHASE;  // Reset to chase mode immediately
                uart_puts("Ghost eaten!\r\n");
            } else if (ghost->state == GHOST_STATE_CHASE) {
                // Spudman dies!
                game->lives--;
                uart_puts("Spudman caught!\r\n");

                if (game->lives == 0) {
                    game->game_over = 1;
                    uart_puts("GAME OVER!\r\n");
                } else {
                    // Reset positions
                    game->spudman.x = 1;
                    game->spudman.y = 13;
                    game->spudman.pixel_x = game->spudman.x * TILE_SIZE;
                    game->spudman.pixel_y = game->spudman.y * TILE_SIZE;
                    game->spudman.direction = DIR_NONE;
                }
            }
        }
    }
}

void game_update(game_state_t* game) {
    if (game->game_over) {
        return;
    }

    game_update_spudman(game);
    game_update_ghosts(game);
    game_check_collisions(game);

    // Update power timer
    if (game->power_timer > 0) {
        game->power_timer--;
        if (game->power_timer == 0) {
            // Ghosts return to chase mode
            uart_puts("Power pellet expired! Ghosts back to chase!\r\n");
            for (int i = 0; i < MAX_GHOSTS; i++) {
                if (game->ghosts[i].state == GHOST_STATE_SCARED) {
                    game->ghosts[i].state = GHOST_STATE_CHASE;
                }
            }
        }
    }

    // Check for level complete
    if (game->pellets_remaining == 0) {
        uart_puts("Level complete!\r\n");
        game_init(game);  // Restart level
        game->lives++;  // Bonus life!
    }

    game->frame_count++;
}

void game_draw_maze(game_state_t* game) {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            int px = x * TILE_SIZE;
            int py = y * TILE_SIZE;

            uint8_t tile = game->maze[y][x];

            if (tile == TILE_WALL) {
                // Draw wall as filled blue square
                display_fill_rect(px, py, TILE_SIZE, TILE_SIZE, COLOR_WALL);
            } else if (tile == TILE_PELLET) {
                // Draw pellet as small green sprout (1 pixel in center)
                display_set_pixel(px + 1, py + 1, COLOR_PELLET);
                display_set_pixel(px + 2, py + 1, COLOR_PELLET);
                display_set_pixel(px + 1, py + 2, COLOR_PELLET);
            } else if (tile == TILE_POWER_PELLET) {
                // Draw power pellet as larger green sprout (2x2)
                display_fill_rect(px + 1, py + 1, 2, 2, COLOR_POWER_PELLET);
            }
        }
    }
}

void game_draw_spudman(game_state_t* game) {
    spudman_t* sp = &game->spudman;

    spud_color_t colors[5] = {
        COLOR_BACKGROUND,   // 0
        COLOR_SPUDMAN,      // 1 - potato body
        0xA0A0A0,           // 2 - lighter brown
        COLOR_SPUDMAN_EYE,  // 3 - eyes
        0x000000            // 4 - mouth
    };

    for (int sy = 0; sy < TILE_SIZE; sy++) {
        for (int sx = 0; sx < TILE_SIZE; sx++) {
            uint8_t pixel = spudman_sprite[sy][sx];
            if (pixel > 0) {
                display_set_pixel(sp->pixel_x + sx, sp->pixel_y + sy, colors[pixel]);
            }
        }
    }
}

void game_draw_ghosts(game_state_t* game) {
    for (int i = 0; i < MAX_GHOSTS; i++) {
        ghost_t* ghost = &game->ghosts[i];

        if (ghost->state == GHOST_STATE_EATEN) {
            continue;  // Don't draw eaten ghosts
        }

        spud_color_t ghost_color = (ghost->state == GHOST_STATE_SCARED) ? COLOR_GHOST_SCARED : ghost->color;

        spud_color_t colors[3] = {
            COLOR_BACKGROUND,  // 0
            ghost_color,       // 1 - ghost body
            COLOR_WHITE        // 2 - eyes
        };

        for (int sy = 0; sy < TILE_SIZE; sy++) {
            for (int sx = 0; sx < TILE_SIZE; sx++) {
                uint8_t pixel = ghost_sprite[sy][sx];
                if (pixel > 0) {
                    display_set_pixel(ghost->pixel_x + sx, ghost->pixel_y + sy, colors[pixel]);
                }
            }
        }
    }
}

void game_draw(game_state_t* game) {
    // Draw maze and pellets
    game_draw_maze(game);

    // Draw ghosts
    game_draw_ghosts(game);

    // Draw spudman (on top)
    game_draw_spudman(game);

    // Draw HUD - score and lives (in unused space or at bottom)
    // For now, we'll use the maze itself as display area
    // Could add score overlay if needed
}

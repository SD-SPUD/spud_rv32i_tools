#include "game.h"

void game_init(game_state_t* game) {
    game->gameOver = false;
    game->dir = DIR_STOP;

    // center the starting position of the snake
    game->x = GRID_WIDTH / 2;
    game->y = GRID_HEIGHT / 2;

    // spawn initial food
    game_spawn_food(game);

    game->score = 0;
    game->nTail = 0;
}

void game_spawn_food(game_state_t* game) {
    // generate random food position
    game->foodX = rand_range(0, GRID_WIDTH - 1);
    game->foodY = rand_range(0, GRID_HEIGHT - 1);

    // ensure food doesn't spawn on snake
    bool on_snake = true;
    while (on_snake) {
        on_snake = false;

        // check if food is on head
        if (game->foodX == game->x && game->foodY == game->y) {
            on_snake = true;
        }

        // check if food is on tail
        for (int i = 0; i < game->nTail; i++) {
            if (game->foodX == game->tailX[i] && game->foodY == game->tailY[i]) {
                on_snake = true;
                break;
            }
        }

        // respawn if on snake
        if (on_snake) {
            game->foodX = rand_range(0, GRID_WIDTH - 1);
            game->foodY = rand_range(0, GRID_HEIGHT - 1);
        }
    }
}

void game_update(game_state_t* game) {
    // update tail positions
    int prevX = game->tailX[0];
    int prevY = game->tailY[0];
    int prev2X, prev2Y;

    game->tailX[0] = game->x;
    game->tailY[0] = game->y;

    for (int i = 1; i < game->nTail; i++) {
        prev2X = game->tailX[i];
        prev2Y = game->tailY[i];
        game->tailX[i] = prevX;
        game->tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // move head based on direction
    switch (game->dir) {
        case DIR_LEFT:  game->x--; break;
        case DIR_RIGHT: game->x++; break;
        case DIR_UP:    game->y--; break;
        case DIR_DOWN:  game->y++; break;
        case DIR_STOP:  break;
    }

    // handle wall wrapping
    if (game->x >= GRID_WIDTH) game->x = 0;
    else if (game->x < 0) game->x = GRID_WIDTH - 1;

    if (game->y >= GRID_HEIGHT) game->y = 0;
    else if (game->y < 0) game->y = GRID_HEIGHT - 1;

    // check for self-collision
    for (int i = 0; i < game->nTail; i++) {
        if (game->tailX[i] == game->x && game->tailY[i] == game->y) {
            game->gameOver = true;
        }
    }

    // check for food collision
    if (game->x == game->foodX && game->y == game->foodY) {
        game->score += 10;
        game->nTail++;
        game_spawn_food(game);
    }
}

void game_draw(game_state_t* game) {
    display_clear(COLOR_BLACK);

    // draw snake head as 4x4 block
    for (int dy = 0; dy < BLOCK_SIZE; dy++) {
        for (int dx = 0; dx < BLOCK_SIZE; dx++) {
            display_set_pixel(game->x * BLOCK_SIZE + dx, game->y * BLOCK_SIZE + dy, COLOR_GREEN);
        }
    }

    // draw tail segments as 4x4 blocks
    for (int i = 0; i < game->nTail; i++) {
        for (int dy = 0; dy < BLOCK_SIZE; dy++) {
            for (int dx = 0; dx < BLOCK_SIZE; dx++) {
                display_set_pixel(game->tailX[i] * BLOCK_SIZE + dx, game->tailY[i] * BLOCK_SIZE + dy, COLOR_YELLOW);
            }
        }
    }

    // draw food as 4x4 block
    for (int dy = 0; dy < BLOCK_SIZE; dy++) {
        for (int dx = 0; dx < BLOCK_SIZE; dx++) {
            display_set_pixel(game->foodX * BLOCK_SIZE + dx, game->foodY * BLOCK_SIZE + dy, COLOR_RED);
        }
    }

    display_update();
}

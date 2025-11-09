#include "board.h"

void tetris_board_init(void) {
    display_init();
}

void tetris_board_draw(void) {
    // Clear the entire display
    display_clear(BACKGROUND_COLOR);

    // Draw the tetris play area (40 pixels wide, 64 pixels tall)
    // We'll draw a simple grid to show the boundaries
    for (uint8_t row = 0; row < BOARD_HEIGHT; row++) {
        for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
            uint8_t x = col * BLOCK_SIZE;
            uint8_t y = row * BLOCK_SIZE;

            // Draw subtle grid lines (just the borders of each block)
            // Top and left edges only to create a grid effect
            display_set_pixel(x, y, GRID_COLOR);
        }
    }
}
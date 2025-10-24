#include "board.h"

void board_init(void) {
    display_init();
}

void board_draw(void) {
    display_clear(COLOR_BLACK);

    for (uint8_t row = 0; row < BOARD_SIZE; row++) {
        for (uint8_t col = 0; col < BOARD_SIZE; col++) {
            uint8_t x = col * SQUARE_SIZE;
            uint8_t y = row * SQUARE_SIZE;

            // determine if square should be light or dark
            uint8_t is_light = (row + col) % 2;

            spud_color_t color = is_light ? LIGHT_SQUARE_COLOR : DARK_SQUARE_COLOR;
            display_fill_rect(x, y, SQUARE_SIZE, SQUARE_SIZE, color);
        }
    }
}
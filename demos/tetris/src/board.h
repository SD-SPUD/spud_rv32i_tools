#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#include "spudkit.h"

// Standard tetris board is 10 wide x 20 tall
// But we have 64x64 display, so we'll use 10 wide x 16 tall
// Each block is 6x4 pixels to fit nicely
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 16
#define BLOCK_SIZE 4  // 4x4 pixels per block (10*4 = 40 width, 16*4 = 64 height)

// board background color
#define BACKGROUND_COLOR COLOR_BLACK
#define GRID_COLOR COLOR_WHITE

// board functions
void board_init(void);
void board_draw(void);

#endif // TETRIS_BOARD_H

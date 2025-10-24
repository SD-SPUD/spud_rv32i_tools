#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "spudkit.h"

#define BOARD_SIZE 8
#define SQUARE_SIZE 8  // 64x64 display / 8 squares = 8 pixels per square

// easily changeable square colors
#define LIGHT_SQUARE_COLOR COLOR_WHITE
#define DARK_SQUARE_COLOR  COLOR_BLACK

// board functions
void board_init(void);
void board_draw(void);

#endif // CHESS_BOARD_H

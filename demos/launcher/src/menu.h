#ifndef LAUNCHER_MENU_H
#define LAUNCHER_MENU_H

#include "spudkit.h"

// Number of games in menu
#define NUM_GAMES 5

// Game IDs
typedef enum {
    GAME_SNAKE = 0,
    GAME_TETRIS = 1,
    GAME_CHESS = 2,
    GAME_SPUDMAN = 3,
    GAME_DONUT = 4
} game_id_t;

// Menu state
typedef struct {
    uint8_t selected_index;
    uint16_t last_buttons;
} menu_state_t;

// Function prototypes
void menu_init(menu_state_t* menu);
void menu_draw(menu_state_t* menu);
int menu_update(menu_state_t* menu);  // Returns selected game ID or -1 if none selected

// External game main functions
extern int snake_main(void);
extern int tetris_main(void);
extern int chess_main(void);
extern int spudman_main(void);
extern int donut_main(void);
extern int sample_main(void);

#endif // LAUNCHER_MENU_H

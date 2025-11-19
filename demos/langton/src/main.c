#include <spudkit.h>

#define GRID_SIZE 64

// grid: 0 = white, 1 = black
uint8_t grid[GRID_SIZE][GRID_SIZE];

// ant state
int ant_x = 32;
int ant_y = 32;
int ant_dir = 0;  // 0=up, 1=right, 2=down, 3=left

// direction offsets
int dx[4] = {0, 1, 0, -1};  // x offset for each direction
int dy[4] = {-1, 0, 1, 0};  // y offset for each direction

void step() {
    // get current cell color
    int cell = grid[ant_y][ant_x];

    // flip the color
    grid[ant_y][ant_x] = 1 - cell;

    // turn based on color
    if (cell == 0) {
        // white: turn right
        ant_dir = (ant_dir + 1) % 4;
    } else {
        // black: turn left
        ant_dir = (ant_dir + 3) % 4;
    }

    // move forward
    ant_x = (ant_x + dx[ant_dir] + GRID_SIZE) % GRID_SIZE;
    ant_y = (ant_y + dy[ant_dir] + GRID_SIZE) % GRID_SIZE;
}

void draw() {
    display_clear(COLOR_BLACK);

    // draw all black cells
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x]) {
                display_set_pixel(x, y, COLOR_WHITE);
            }
        }
    }

    // draw ant in red
    display_set_pixel(ant_x, ant_y, COLOR_RED);

    display_update();
}

int main() {
    spudkit_init();
    display_clear(COLOR_BLACK);


    while (1) {
        draw();
        step();
    }

    return 0;
}

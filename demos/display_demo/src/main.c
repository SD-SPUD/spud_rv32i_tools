/**
 * a 3d spining donut
 */

#include "spudkit.h"

int display_demo_main() {
    // initialize the spudkit library
    spudkit_init();
    sim_puts("\r\nSpudKit Display Demo\r\n");
    sim_puts("===================\r\n\r\n");
    uart_puts("\r\nSpudKit Display Demo\r\n");
    uart_puts("===================\r\n\r\n");

    // seed random number generator with a simple value
    rand_seed(0x12345678);

    // color palette for random shapes
    spud_color_t colors[] = {
        COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW,
        COLOR_CYAN, COLOR_MAGENTA, COLOR_WHITE
    };
    uint8_t num_colors = 7;

    uint32_t frame = 0;
    while(1) {
        // clear display for fresh frame
        display_clear(COLOR_BLACK);

        // generate 3-5 random lines
        uint8_t num_lines = rand_range(3, 6);
        for(uint8_t i = 0; i < num_lines; i++) {
            sim_puts("making lines");
            uint8_t x1 = rand_range(0, 64);
            uint8_t y1 = rand_range(0, 64);
            uint8_t x2 = rand_range(0, 64);
            uint8_t y2 = rand_range(0, 64);
            spud_color_t color = colors[rand_range(0, num_colors)];
            display_draw_line(x1, y1, x2, y2, color);
        }

        // generate 2-4 random rectangles
        uint8_t num_rects = rand_range(2, 5);
        for(uint8_t i = 0; i < num_rects; i++) {
            uint8_t x = rand_range(0, 50);
            uint8_t y = rand_range(0, 50);
            uint8_t w = rand_range(5, 15);
            uint8_t h = rand_range(5, 15);
            spud_color_t color = colors[rand_range(0, num_colors)];

            if(rand_range(0, 2)) {
                display_fill_rect(x, y, w, h, color);
            } else {
                display_draw_rect(x, y, w, h, color);
            }
        }

        // generate 1-3 random circles
        uint8_t num_circles = rand_range(1, 4);
        for(uint8_t i = 0; i < num_circles; i++) {
            uint8_t x = rand_range(10, 54);
            uint8_t y = rand_range(10, 54);
            uint8_t r = rand_range(3, 12);
            spud_color_t color = colors[rand_range(0, num_colors)];
            display_draw_circle(x, y, r, color);
        }

        // add some random pixels for sparkle effect
        uint8_t num_pixels = rand_range(5, 15);
        for(uint8_t i = 0; i < num_pixels; i++) {
            uint8_t x = rand_range(0, 64);
            uint8_t y = rand_range(0, 64);
            spud_color_t color = colors[rand_range(0, num_colors)];
            display_set_pixel(x, y, color);
        }

        // update display to show new frame
        display_update();

        // print status every 50 frames (using manual check to avoid modulo)
        if(frame >= 50 && (frame - 50) < 1) {
            uart_puts("Reactive frame: ");
            sim_put_dec(frame);
            uart_puts("\r\n");
        } else if(frame >= 100 && (frame - 100) < 1) {
            uart_puts("Reactive frame: ");
            sim_put_dec(frame);
            uart_puts("\r\n");
        } else if(frame >= 150 && (frame - 150) < 1) {
            uart_puts("Reactive frame: ");
            sim_put_dec(frame);
            uart_puts("\r\n");
        }

        delay_ms(10);  // ~10 FPS for readable animation
        frame++;
    }

    return 0;
}

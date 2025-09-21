/**
  * a 3D spinning donut
  */
#include "spudkit.h"

// radius of circle
#define R1 20

int main() {
    // initialize the spudkit library
    spudkit_init();
    uart_puts("\r\nSpudKit DONUT Demo\r\n");
    uart_puts("===================\r\n\r\n");

    // seed random number generator with a simple value
    rand_seed(0x12345678);

    // color palette for random shapes
    spud_color_t colors[] = {
        COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW,
        COLOR_CYAN, COLOR_MAGENTA, COLOR_WHITE
    };
    uint8_t num_colors = 7;
    while(1) {

        // draw a circle with debug output
        for(ffloat theta = FFLOAT(0); theta < FF2PI; theta += FFLOAT(0.1)) {
            /*
            ffprint(ffsin(theta));
            uart_puts(", ");
            ffprint(ffcos(theta));
            uart_puts("\r\n");
            */

            struct ffvect3_t vect = { FFLOAT(R1), FFLOAT(0), FFLOAT(0) };

            // rotate on the z axis
            vect = ROTATEZ_VECT3(vect, theta);

            // translate to center of screen (32, 32)
            int screen_x = FFINT(vect.x) + 32;
            int screen_y = FFINT(vect.y) + 32;

            ffvect3_print(vect);
            uart_puts("\r\n");

            // clamp to screen bounds
            if (screen_x >= 0 && screen_x < 64 && screen_y >= 0 && screen_y < 64) {
                display_set_pixel(screen_x, screen_y, COLOR_RED);
            }
        }

        display_update();
        delay_ms(10);
    }
    return 0;

}

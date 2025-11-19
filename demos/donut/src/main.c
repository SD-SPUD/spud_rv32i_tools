/**
  * a 3D spinning donut
  */

#include "spudkit.h"

// radius of circle
#define R1 10
#define R2 20

// K scaling constants
#define K2 FFLOAT(80)  // distance from viewer
#define K1 FFLOAT(60)  // screen scaling factor

int donut_main() {
    // initialize the spudkit library
    spudkit_init();
    uart_puts("\r\nSpudKit DONUT Demo\r\n");
    uart_puts("===================\r\n\r\n");
    uart_puts("Press SELECT button to exit to menu\r\n");

    // Initialize arcade buttons
    arcade_init();
    uint16_t last_buttons = arcade_read_all();

    // seed random number generator with a simple value
    rand_seed(0x12345678);
    while(1) {
        // Check for SELECT button press to exit
        uint16_t buttons = arcade_read_all();
        uint16_t button_pressed = buttons & ~last_buttons;
        if (button_pressed & (1 << ARCADE_BUTTON_SELECT)) {
            uart_puts("Exiting to menu...\r\n");
            break;
        }
        last_buttons = buttons;

        static ffloat spin = 0;
        static ffloat spin2 = 0;
        display_clear(0);
        // draw a circle with debug output
            for(ffloat phi = FFLOAT(0); phi < FF2PI; phi += FFLOAT(0.2)) {
                for(ffloat theta = FFLOAT(0); theta < FF2PI; theta += FFLOAT(0.6)) {
                struct ffvect3_t vect = { FFLOAT(R1), FFLOAT(0), FFLOAT(0) };

                // rotate on the z axis
                vect = ROTATEZ_VECT3(vect, theta);
                // rotate on the y axis
                vect.y += FFLOAT(R2);
                vect = ROTATEX_VECT3(vect, phi);

                // add some fun spin
                vect = ROTATEY_VECT3(vect, spin);
                vect = ROTATEZ_VECT3(vect, spin2);
                // K scaling projection
                ffloat z = vect.z + K2;
                ffloat ooz = FFDIV(FFLOAT(1), z);  // one over z
                int screen_x = 32 + FFINT(FFMULT(K1, FFMULT(ooz, vect.x)));
                int screen_y = 32 - FFINT(FFMULT(K1, FFMULT(ooz, vect.y)));  // note the minus for Y


                // clamp to screen bounds
                if (screen_x >= 0 && screen_x < 64 && screen_y >= 0 && screen_y < 64) {
                    // Calculate luminance based on Z depth
                    ffloat luminance = FFMULT(vect.z, FFLOAT(0.1)) + FFLOAT(0.5);
                    int color_intensity = FFINT(FFMULT(luminance, FFLOAT(7)));
                    // Clamp to valid array bounds (0-7)
                    if (color_intensity < 0) color_intensity = 0;
                    if (color_intensity > 7) color_intensity = 7;

                    // Purple hues from dark to bright
                    uint16_t purple_shades[] = {
                        0x6010,  // Dark purple (R=12, G=0, B=16)
                        0x7011,  // Darker purple (R=14, G=0, B=17)
                        0x8012,  // Medium-dark purple (R=16, G=0, B=18)
                        0x9013,  // Medium purple (R=18, G=0, B=19)
                        0xA014,  // Medium-bright purple (R=20, G=0, B=20)
                        0xB015,  // Bright purple (R=22, G=0, B=21)
                        0xC016,  // Brighter purple (R=24, G=0, B=22)
                        0xD017   // Brightest purple (R=26, G=0, B=23)
                    };
                    display_set_pixel(screen_x, screen_y, purple_shades[color_intensity]);
                }
            }
        }
        spin = (spin >= FF2PI) ? 0 : spin + FFLOAT(0.05);
        spin2 = (spin2 >= FF2PI) ? 0 : spin2 + FFLOAT(0.01);
        display_update();
    }
    return 0;

}

int main(){
    return donut_main();
}

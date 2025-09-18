#include "display.h"
#include "spudkit.h"
#ifdef UART_DISPLAY
#include "uart.h"
#endif

// Conditional macros for display output - switch between SIM and UART modes
#ifdef UART_DISPLAY
    #define DISPLAY_PUTS(str)           uart_puts(str)
    #define DISPLAY_PUTC(ch)            uart_putc(ch)
    #define DISPLAY_PUT_HEX16(val)      uart_put_hex16(val)
    #define DISPLAY_CURSOR_HOME()       uart_cursor_home()
    #define DISPLAY_CLEAR_SCREEN()      uart_clear_screen()
    #define DISPLAY_SET_COLOR(fg, bg)   uart_set_color(fg, bg)
    #define DISPLAY_RESET_COLOR()       uart_reset_color()
    #define DISPLAY_RGB565_TO_ANSI(rgb) uart_rgb565_to_ansi(rgb)
#elif defined(SIM_DISPLAY)
    #define DISPLAY_PUTS(str)           sim_puts(str)
    #define DISPLAY_PUTC(ch)            sim_putc(ch)
    #define DISPLAY_PUT_HEX16(val)      sim_put_hex16(val)
    #define DISPLAY_CURSOR_HOME()       sim_cursor_home()
    #define DISPLAY_CLEAR_SCREEN()      sim_clear_screen()
    #define DISPLAY_SET_COLOR(fg, bg)   sim_set_color(fg, bg)
    #define DISPLAY_RESET_COLOR()       sim_reset_color()
    #define DISPLAY_RGB565_TO_ANSI(rgb) sim_rgb565_to_ansi(rgb)
#else
    // No display output - stub functions
    #define DISPLAY_PUTS(str)
    #define DISPLAY_PUTC(ch)
    #define DISPLAY_PUT_HEX16(val)
    #define DISPLAY_CURSOR_HOME()
    #define DISPLAY_CLEAR_SCREEN()
    #define DISPLAY_SET_COLOR(fg, bg)
    #define DISPLAY_RESET_COLOR()
    #define DISPLAY_RGB565_TO_ANSI(rgb) 0
#endif

// frame buffer - stores the entire display in memory
static spud_color_t framebuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH];

// display functions for both SIM_DISPLAY and UART_DISPLAY modes
#if defined(SIM_DISPLAY) || defined(UART_DISPLAY)

void display_sim_print_pixel(uint8_t x, uint8_t y, spud_color_t color) {
    DISPLAY_PUTS("PIXEL(");
    DISPLAY_PUT_HEX16(x);
    DISPLAY_PUTC(',');
    DISPLAY_PUT_HEX16(y);
    DISPLAY_PUTS(")=0x");
    DISPLAY_PUT_HEX16(color);
    DISPLAY_PUTS("\r\n");
}

void display_sim_print_buffer(void) {
    // move cursor to top-left to redraw over previous display
    DISPLAY_CURSOR_HOME();

    // draw top border
    DISPLAY_RESET_COLOR();
    DISPLAY_PUTS("┌");
    for(int x = 0; x < DISPLAY_WIDTH; x++) {
        DISPLAY_PUTS("─");
    }
    DISPLAY_PUTS("┐\r\n");

    // draw the 64x64 display with half-height blocks (2 pixels per character)
    for(int y = 0; y < DISPLAY_HEIGHT; y += 2) {
        DISPLAY_RESET_COLOR();
        DISPLAY_PUTS("│"); // left border

        for(int x = 0; x < DISPLAY_WIDTH; x++) {
            spud_color_t top_pixel = framebuffer[y][x];
            spud_color_t bottom_pixel = (y + 1 < DISPLAY_HEIGHT) ? framebuffer[y + 1][x] : COLOR_BLACK;

            if(top_pixel == COLOR_BLACK && bottom_pixel == COLOR_BLACK) {
                // both pixels black - use space
                DISPLAY_RESET_COLOR();
                DISPLAY_PUTS(" ");
            } else if(top_pixel != COLOR_BLACK && bottom_pixel == COLOR_BLACK) {
                // top pixel colored, bottom black - upper half block
                uint8_t ansi_color = DISPLAY_RGB565_TO_ANSI(top_pixel);
                DISPLAY_SET_COLOR(ansi_color, 16); // colored fg, black bg
                DISPLAY_PUTS("▀");
                DISPLAY_RESET_COLOR();
            } else if(top_pixel == COLOR_BLACK && bottom_pixel != COLOR_BLACK) {
                // top black, bottom colored - lower half block
                uint8_t ansi_color = DISPLAY_RGB565_TO_ANSI(bottom_pixel);
                DISPLAY_SET_COLOR(ansi_color, 16); // colored fg, black bg
                DISPLAY_PUTS("▄");
                DISPLAY_RESET_COLOR();
            } else {
                // both pixels colored
                uint8_t top_color = DISPLAY_RGB565_TO_ANSI(top_pixel);
                uint8_t bottom_color = DISPLAY_RGB565_TO_ANSI(bottom_pixel);
                DISPLAY_SET_COLOR(top_color, bottom_color); // top as fg, bottom as bg
                DISPLAY_PUTS("▀");
                DISPLAY_RESET_COLOR();
            }
        }

        DISPLAY_RESET_COLOR();
        DISPLAY_PUTS("│\r\n"); // Right border and newline
    }

    // Draw bottom border
    DISPLAY_RESET_COLOR();
    DISPLAY_PUTS("└");
    for(int x = 0; x < DISPLAY_WIDTH; x++) {
        DISPLAY_PUTS("─");
    }
    DISPLAY_PUTS("┘\r\n");

    DISPLAY_RESET_COLOR();
}

#endif

void display_init(void) {
    // initialize framebuffer to black
    mem_set(framebuffer, 0, sizeof(framebuffer));

#if defined(SIM_DISPLAY) || defined(UART_DISPLAY)
    DISPLAY_CLEAR_SCREEN();
    #ifdef UART_DISPLAY
    DISPLAY_PUTS("DISPLAY_INIT: UART display mode enabled - Visual Terminal Display\r\n");
    #else
    DISPLAY_PUTS("DISPLAY_INIT: Simulation mode enabled - Visual Terminal Display\r\n");
    #endif
    DISPLAY_PUTS("Display size: 64x64 pixels\r\n\r\n");
#else
    // todo: initialize actual 64x64 matrix display hardware
    // this will involve setting up the display controller registers
    // at SPUD_DISPLAY_BASE when hardware is implemented
#endif
}

void display_clear(spud_color_t color) {
    for(int y = 0; y < DISPLAY_HEIGHT; y++) {
        for(int x = 0; x < DISPLAY_WIDTH; x++) {
            framebuffer[y][x] = color;
        }
    }

#if defined(SIM_DISPLAY) || defined(UART_DISPLAY)
    DISPLAY_PUTS("DISPLAY_CLEAR: color=0x");
    DISPLAY_PUT_HEX16(color);
    DISPLAY_PUTS("\r\n");
#endif
}

void display_set_pixel(uint8_t x, uint8_t y, spud_color_t color) {
    if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;

    framebuffer[y][x] = color;

#if defined(SIM_DISPLAY) || defined(UART_DISPLAY)
    // display_sim_print_pixel(x, y, color);
#endif
}

spud_color_t display_get_pixel(uint8_t x, uint8_t y) {
    if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return COLOR_BLACK;
    return framebuffer[y][x];
}

void display_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, spud_color_t color) {
    // Bresenham's line algorithm
    int dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int dy = y1 > y0 ? y1 - y0 : y0 - y1;
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while(1) {
        display_set_pixel(x0, y0, color);

        if(x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if(e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if(e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void display_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, spud_color_t color) {
    // Draw rectangle outline
    display_draw_line(x, y, x + width - 1, y, color);                    // Top
    display_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color); // Bottom
    display_draw_line(x, y, x, y + height - 1, color);                  // Left
    display_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color);  // Right
}

void display_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, spud_color_t color) {
    for(uint8_t py = y; py < y + height && py < DISPLAY_HEIGHT; py++) {
        for(uint8_t px = x; px < x + width && px < DISPLAY_WIDTH; px++) {
            display_set_pixel(px, py, color);
        }
    }
}

void display_draw_circle(uint8_t cx, uint8_t cy, uint8_t radius, spud_color_t color) {
    // Bresenham's circle algorithm
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while(y >= x) {
        // Draw 8 symmetrical points
        display_set_pixel(cx + x, cy + y, color);
        display_set_pixel(cx - x, cy + y, color);
        display_set_pixel(cx + x, cy - y, color);
        display_set_pixel(cx - x, cy - y, color);
        display_set_pixel(cx + y, cy + x, color);
        display_set_pixel(cx - y, cy + x, color);
        display_set_pixel(cx + y, cy - x, color);
        display_set_pixel(cx - y, cy - x, color);

        x++;
        if(d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

// Simple 8x8 font (basic ASCII characters)
static const uint8_t font_8x8[96][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    {0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00}, // !
    {0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // "
    // ... (more characters would be added here)
    // For now, just implement a few basic characters
};

void display_draw_char(uint8_t x, uint8_t y, char c, spud_color_t color, spud_color_t bg_color) {
    if(c < 32 || c > 126) c = 32; // Default to space for unsupported chars

    const uint8_t* char_data = font_8x8[c - 32];

    for(int py = 0; py < 8; py++) {
        uint8_t row = char_data[py];
        for(int px = 0; px < 8; px++) {
            spud_color_t pixel_color = (row & (0x80 >> px)) ? color : bg_color;
            if(x + px < DISPLAY_WIDTH && y + py < DISPLAY_HEIGHT) {
                display_set_pixel(x + px, y + py, pixel_color);
            }
        }
    }
}

void display_draw_string(uint8_t x, uint8_t y, const char* str, spud_color_t color, spud_color_t bg_color) {
    uint8_t cx = x;
    uint8_t cy = y;

    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == '\n') {
            cx = x;
            cy += 8;
            if(cy >= DISPLAY_HEIGHT) break;
            continue;
        }

        display_draw_char(cx, cy, str[i], color, bg_color);
        cx += 8;

        if(cx >= DISPLAY_WIDTH) {
            cx = x;
            cy += 8;
            if(cy >= DISPLAY_HEIGHT) break;
        }
    }
}

void display_update(void) {
#if defined(SIM_DISPLAY) || defined(UART_DISPLAY)
    DISPLAY_PUTS("DISPLAY_UPDATE: Refreshing display\r\n");
    display_sim_print_buffer();
#else
    // TODO: Transfer framebuffer to actual 64x64 matrix display
    // This will involve writing the framebuffer data to the display controller
    // registers at SPUD_DISPLAY_BASE when hardware is implemented

    // For now, just a placeholder that would write to display hardware
    volatile uint32_t* display_ctrl = (volatile uint32_t*)SPUD_DISPLAY_BASE;

    // Example of how this might work:
    // for(int i = 0; i < DISPLAY_PIXELS; i++) {
    //     display_ctrl[i] = framebuffer[i / DISPLAY_WIDTH][i % DISPLAY_WIDTH];
    // }

    (void)display_ctrl; // Suppress unused variable warning
#endif
}

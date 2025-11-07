#ifndef SPUDKIT_DISPLAY_H
#define SPUDKIT_DISPLAY_H

#include <stdint.h>

// display dimensions
#define DISPLAY_WIDTH  64
#define DISPLAY_HEIGHT 64
#define DISPLAY_PIXELS (DISPLAY_WIDTH * DISPLAY_HEIGHT)

// color format switch based on simulation vs real hardware
#ifdef SIM_DISPLAY
    // use RGB565 format for simulation (16-bit)
    typedef uint16_t spud_color_t;

    #define COLOR_BLACK   0x0000
    #define COLOR_WHITE   0xFFFF
    #define COLOR_RED     0xF800
    #define COLOR_GREEN   0x07E0
    #define COLOR_BLUE    0x001F
    #define COLOR_YELLOW  0xFFE0
    #define COLOR_CYAN    0x07FF
    #define COLOR_MAGENTA 0xF81F
#else
    // use RGB888 format for real hardware (24-bit 0xFFFFFF)
    typedef uint32_t spud_color_t;

    #define COLOR_BLACK   0x000000
    #define COLOR_WHITE   0xFFFFFF
    #define COLOR_RED     0x0000FF
    #define COLOR_GREEN   0x00FF00
    #define COLOR_BLUE    0xFF0000
    #define COLOR_YELLOW  0x00FFFF
    #define COLOR_CYAN    0xFFFF00
    #define COLOR_MAGENTA 0xFF00FF
#endif

// display initialization
void display_init(void);

// basic drawing functions
void display_clear(spud_color_t color);
void display_set_pixel(uint8_t x, uint8_t y, spud_color_t color);
spud_color_t display_get_pixel(uint8_t x, uint8_t y);

// drawing primitives
void display_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, spud_color_t color);
void display_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, spud_color_t color);
void display_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, spud_color_t color);
void display_draw_circle(uint8_t x, uint8_t y, uint8_t radius, spud_color_t color);

// text rendering (simple 8x8 font)
void display_draw_char(uint8_t x, uint8_t y, char c, spud_color_t color, spud_color_t bg_color);
void display_draw_string(uint8_t x, uint8_t y, const char* str, spud_color_t color, spud_color_t bg_color);

// text rendering with scale (scale 1 = 8x8, scale 2 = 16x16, etc.)
void display_draw_char_scaled(uint8_t x, uint8_t y, char c, spud_color_t color, spud_color_t bg_color, uint8_t scale);
void display_draw_string_scaled(uint8_t x, uint8_t y, const char* str, spud_color_t color, spud_color_t bg_color, uint8_t scale);

// text rendering with small 4x6 font
void display_draw_char_small(uint8_t x, uint8_t y, char c, spud_color_t color, spud_color_t bg_color);
void display_draw_string_small(uint8_t x, uint8_t y, const char* str, spud_color_t color, spud_color_t bg_color);

// color conversion utility
spud_color_t display_map_rgb565_to_palette(uint16_t color_565);

// display update
void display_update(void);

// simulation helpers (when --sim-display is enabled)
#ifdef SIM_DISPLAY
void display_sim_print_buffer(void);
void display_sim_print_pixel(uint8_t x, uint8_t y, spud_color_t color);
#endif

#endif // SPUDKIT_DISPLAY_H

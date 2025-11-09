#include "spudkit.h"

// Button colors (RGB565 format)
#define COLOR_RELEASED  COLOR_BLACK
#define COLOR_PRESSED   COLOR_WHITE

// Button-specific colors for variety
const spud_color_t button_colors[10] = {
    COLOR_RED,      // LEFT - Red
    COLOR_GREEN,    // DOWN - Green
    COLOR_BLUE,     // RIGHT - Blue
    COLOR_YELLOW,   // UP - Yellow
    COLOR_MAGENTA,  // X - Magenta
    COLOR_CYAN,     // Y - Cyan
    0xF81F,         // A - Pink
    0x07FF,         // B - Light Blue
    0xFFE0,         // START - Light Yellow
    0xF800          // SELECT - Dark Red
};

// Button areas on 64x64 display
typedef struct {
    uint8_t x, y, width, height;
    const char* name;
} button_area_t;

const button_area_t button_areas[10] = {
    {0,  0,  16, 16, "LEFT"},   // Top row
    {16, 0,  16, 16, "DOWN"},
    {32, 0,  16, 16, "RIGHT"},
    {48, 0,  16, 16, "UP"},

    {0,  18, 16, 16, "X"},      // Second row - moved up slightly
    {16, 18, 16, 16, "Y"},
    {32, 18, 16, 16, "A"},
    {48, 18, 16, 16, "B"},

    {6,  36, 20, 14, "START"},  // After action buttons, bigger
    {38, 36, 20, 14, "SELECT"}  // After action buttons, bigger
};

void draw_button_area(uint8_t button_index, spud_color_t color) {
    const button_area_t* area = &button_areas[button_index];

    // Fill the button area with the specified color
    for (int y = area->y; y < area->y + area->height; y++) {
        for (int x = area->x; x < area->x + area->width; x++) {
            if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
                display_set_pixel(x, y, color);
            }
        }
    }

    // Draw a border around the button area
    spud_color_t border_color = (color == COLOR_RELEASED) ? 0x4208 : COLOR_WHITE; // Dark gray or white

    // Top and bottom borders
    for (int x = area->x; x < area->x + area->width && x < DISPLAY_WIDTH; x++) {
        if (area->y < DISPLAY_HEIGHT) display_set_pixel(x, area->y, border_color);
        if (area->y + area->height - 1 < DISPLAY_HEIGHT) display_set_pixel(x, area->y + area->height - 1, border_color);
    }

    // Left and right borders
    for (int y = area->y; y < area->y + area->height && y < DISPLAY_HEIGHT; y++) {
        if (area->x < DISPLAY_WIDTH) display_set_pixel(area->x, y, border_color);
        if (area->x + area->width - 1 < DISPLAY_WIDTH) display_set_pixel(area->x + area->width - 1, y, border_color);
    }
}

void draw_button_label(uint8_t button_index, spud_color_t text_color) {
    const button_area_t* area = &button_areas[button_index];

    // Calculate text length manually for the two large buttons
    uint8_t text_len = (button_index == 8) ? 5 : 6; // "START" = 5, "SELECT" = 6

    // Calculate center position for text
    uint8_t text_x = area->x + (area->width - text_len * 8) / 2;  // 8 pixels per char
    uint8_t text_y = area->y + (area->height - 8) / 2;

    // Only draw label for larger buttons (START/SELECT)
    if (area->width >= 24) {
        display_draw_string(text_x, text_y, area->name, text_color, COLOR_BLACK);
    }
}

void update_display_from_buttons(uint16_t buttons, uint16_t last_buttons) {
    for (int i = 0; i < 10; i++) {
        uint8_t current_state = (buttons >> i) & 1;
        uint8_t last_state = (last_buttons >> i) & 1;

        // Only update if button state changed
        if (current_state != last_state) {
            spud_color_t color = current_state ? button_colors[i] : COLOR_RELEASED;
            draw_button_area(i, color);

            // Draw label for larger buttons
            if (button_areas[i].width >= 24) {
                spud_color_t text_color = current_state ? COLOR_BLACK : 0x4208; // Black or dark gray
                draw_button_label(i, text_color);
            }
        }
    }
}

void draw_static_elements() {
    // Draw title in middle area
    display_draw_string(2, 40, "ARCADE DISPLAY TEST", COLOR_WHITE, COLOR_BLACK);

    // Draw small labels for d-pad buttons (underneath each button)
    display_draw_char(6,  17, 'L', COLOR_WHITE, COLOR_BLACK);  // LEFT label
    display_draw_char(22, 17, 'D', COLOR_WHITE, COLOR_BLACK);  // DOWN label
    display_draw_char(38, 17, 'R', COLOR_WHITE, COLOR_BLACK);  // RIGHT label
    display_draw_char(54, 17, 'U', COLOR_WHITE, COLOR_BLACK);  // UP label

    // Draw small labels for action buttons (underneath each button)
    display_draw_char(6,  35, 'X', COLOR_WHITE, COLOR_BLACK);  // X label
    display_draw_char(22, 35, 'Y', COLOR_WHITE, COLOR_BLACK);  // Y label
    display_draw_char(38, 35, 'A', COLOR_WHITE, COLOR_BLACK);  // A label
    display_draw_char(54, 35, 'B', COLOR_WHITE, COLOR_BLACK);  // B label
}

int arcade_display_main() {
    // Initialize spudkit library
    spudkit_init();

    // Initialize display and arcade buttons
    display_init();
    arcade_init();

    uart_puts("\r\nArcade Display Test\r\n");
    uart_puts("===================\r\n");
    uart_puts("Press buttons to see colored squares on the 64x64 display!\r\n");
    uart_puts("Each button maps to a different color and screen area.\r\n\r\n");

    // Clear display to black
    display_clear(COLOR_BLACK);

    // Draw all button areas in released state initially
    for (int i = 0; i < 10; i++) {
        draw_button_area(i, COLOR_RELEASED);
    }

    // Draw static text elements
    draw_static_elements();

    // Update the display hardware
    display_update();

    uint16_t last_buttons = 0xFFFF; // Force initial state

    while(1) {
        // Read current button states
        uint16_t buttons = arcade_read_all();

        // Update display if any buttons changed
        if (buttons != last_buttons) {
            update_display_from_buttons(buttons, last_buttons);
            display_update(); // Push changes to hardware

            // Also print to UART for debugging
            uart_puts("Buttons changed: 0x");
            uart_put_hex16(buttons);
            uart_puts("\r\n");

            last_buttons = buttons;
        }

        // Small delay to prevent overwhelming the system
        for (volatile int i = 0; i < 1000; i++);
    }

    return 0;
}

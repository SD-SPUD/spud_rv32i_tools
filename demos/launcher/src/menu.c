#include "menu.h"

// Game names for display
static const char* game_names[NUM_GAMES] = {
    "SNAKE",
    "TETRIS",
    "CHESS",
    "SPUDMAN",
    "SANDBOX"
};

void menu_init(menu_state_t* menu) {
    menu->selected_index = 0;
    arcade_init();
    menu->last_buttons = arcade_read_all();
}

void menu_draw(menu_state_t* menu) {
    // Clear screen
    display_clear(COLOR_BLACK);

    // Draw title "SPUD DEMOS" at top (centered)
    display_draw_string_small(4, 4, "SPUD DEMOS", COLOR_YELLOW, COLOR_BLACK);

    // Draw separator line
    for (uint8_t x = 0; x < 64; x++) {
        display_set_pixel(x, 13, COLOR_YELLOW);
    }

    // Draw game list starting at y=16
    uint8_t y_offset = 16;
    for (uint8_t i = 0; i < NUM_GAMES; i++) {
        spud_color_t text_color = COLOR_WHITE;
        spud_color_t bg_color = COLOR_BLACK;

        // Highlight selected item
        if (i == menu->selected_index) {
            text_color = COLOR_BLACK;
            bg_color = COLOR_GREEN;

            // Draw selection box
            display_fill_rect(2, y_offset - 1, 60, 9, bg_color);

            // Draw arrow indicator
            display_draw_string(2, y_offset, ">", COLOR_BLACK, bg_color);
        }

        // Draw game name (offset by 10 pixels for arrow space)
        display_draw_string(10, y_offset, game_names[i], text_color, bg_color);

        y_offset += 10;  // Space between items
    }

    display_update();
}

int menu_update(menu_state_t* menu) {
    // Read arcade buttons
    uint16_t buttons = arcade_read_all();
    uint16_t button_pressed = buttons & ~menu->last_buttons;  // Detect rising edge

    // Handle UP button - move selection up
    if (button_pressed & (1 << ARCADE_BUTTON_UP)) {
        if (menu->selected_index > 0) {
            menu->selected_index--;
        } else {
            menu->selected_index = NUM_GAMES - 1;  // Wrap around to bottom
        }
    }

    // Handle DOWN button - move selection down
    if (button_pressed & (1 << ARCADE_BUTTON_DOWN)) {
        if (menu->selected_index < NUM_GAMES - 1) {
            menu->selected_index++;
        } else {
            menu->selected_index = 0;  // Wrap around to top
        }
    }

    // Handle A button - select game
    if (button_pressed & (1 << ARCADE_BUTTON_A)) {
        menu->last_buttons = buttons;
        return menu->selected_index;  // Return selected game
    }

    menu->last_buttons = buttons;

    // Check for UART keyboard input
    if (uart_available()) {
        char c = uart_getc();

        switch(c) {
            case 'w':
            case 'W':
                if (menu->selected_index > 0) {
                    menu->selected_index--;
                } else {
                    menu->selected_index = NUM_GAMES - 1;
                }
                break;
            case 's':
            case 'S':
                if (menu->selected_index < NUM_GAMES - 1) {
                    menu->selected_index++;
                } else {
                    menu->selected_index = 0;
                }
                break;
            case '\r':  // Enter key
            case '\n':
            case 'a':
            case 'A':
            case ' ':  // Space
                return menu->selected_index;  // Return selected game
        }
    }

    return -1;  // No selection made
}

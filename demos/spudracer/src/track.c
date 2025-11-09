#include "track.h"

void track_init(void) {
    display_init();
}

void track_draw(game_state_t* game) {
    uint16_t scroll_offset = game->player.distance % 8;

    // Define horizon line (middle of screen for perspective)
    int horizon_y = SCREEN_HEIGHT / 3;  // Horizon at top third

    // Draw sky (top portion)
    for (int y = 0; y < horizon_y; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            display_set_pixel(x, y, 0x5050FF);  // Sky blue
        }
    }

    // Draw pseudo-3D road with perspective using ffloat
    for (int y = horizon_y; y < SCREEN_HEIGHT; y++) {
        // Calculate road width at this scanline (perspective)
        // Road gets wider as we go down (closer to viewer)
        ffloat depth = FFDIV(FFLOAT(y - horizon_y), FFLOAT(SCREEN_HEIGHT - horizon_y));
        int road_width = FFINT(FFLOAT(8) + FFMULT(depth, FFLOAT(48)));  // 8 pixels at horizon, 56 at bottom
        int road_left = (SCREEN_WIDTH - road_width) / 2;
        int road_right = road_left + road_width;

        // Draw grass on sides
        for (int x = 0; x < road_left; x++) {
            display_set_pixel(x, y, COLOR_GRASS_GREEN);
        }
        for (int x = road_right; x < SCREEN_WIDTH; x++) {
            display_set_pixel(x, y, COLOR_GRASS_GREEN);
        }

        // Draw road surface
        for (int x = road_left; x < road_right; x++) {
            display_set_pixel(x, y, COLOR_ROAD_GRAY);
        }

        // Draw lane markings (dashed lines) with perspective
        if (road_width > 16) {  // Only draw lanes when road is wide enough
            int lane1_x = road_left + road_width / 3;
            int lane2_x = road_left + (road_width * 2) / 3;

            // Animated dashed line
            if (((y + scroll_offset) % 8) < 4) {
                if (lane1_x > road_left && lane1_x < road_right) {
                    display_set_pixel(lane1_x, y, COLOR_LANE_YELLOW);
                }
                if (lane2_x > road_left && lane2_x < road_right) {
                    display_set_pixel(lane2_x, y, COLOR_LANE_YELLOW);
                }
            }
        }

        // Draw road edges (white lines)
        if (road_left >= 0 && road_left < SCREEN_WIDTH) {
            display_set_pixel(road_left, y, COLOR_LANE_WHITE);
        }
        if (road_right >= 0 && road_right < SCREEN_WIDTH) {
            display_set_pixel(road_right, y, COLOR_LANE_WHITE);
        }
    }
}

void track_draw_landmarks(game_state_t* game) {
    int horizon_y = SCREEN_HEIGHT / 3;

    for (int i = 0; i < MAX_LANDMARKS; i++) {
        if (!game->landmarks[i].active) continue;

        int ly = game->landmarks[i].y;

        // Skip if above horizon
        if (ly < horizon_y) continue;

        switch (game->landmarks[i].type) {
            case LANDMARK_POND:
                // Draw reflection pond in distance (small circle)
                if (ly >= 0 && ly < SCREEN_HEIGHT - 15) {
                    // Draw pond (circle)
                    for (int r = 0; r < 8; r++) {
                        for (int a = 0; a < 360; a += 30) {
                            int px = SCREEN_WIDTH / 2 + (r * 1) / 2;
                            int py = ly + 10 + r / 2;
                            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                                display_set_pixel(px, py, COLOR_WATER_BLUE);
                            }
                        }
                    }
                    // Fountain in center
                    display_set_pixel(SCREEN_WIDTH / 2, ly + 10, COLOR_FOUNTAIN_WHITE);
                    display_set_pixel(SCREEN_WIDTH / 2, ly + 9, COLOR_FOUNTAIN_WHITE);
                    display_set_pixel(SCREEN_WIDTH / 2 + 1, ly + 10, COLOR_FOUNTAIN_WHITE);
                    display_set_pixel(SCREEN_WIDTH / 2 - 1, ly + 10, COLOR_FOUNTAIN_WHITE);
                }
                break;

            case LANDMARK_HEC:
                // Draw HEC 438 building and sign
                if (ly >= 0 && ly < SCREEN_HEIGHT - 20) {
                    // Building
                    display_fill_rect(2, ly, 12, 18, COLOR_BUILDING_GRAY);
                    // Sign with text
                    display_draw_string(4, ly + 5, "HEC", COLOR_WHITE, COLOR_BLACK);
                    display_draw_string(4, ly + 13, "438", COLOR_WHITE, COLOR_BLACK);
                }
                break;

            case LANDMARK_FLAG:
                // American flag on lawn
                if (ly >= 0 && ly < SCREEN_HEIGHT - 15) {
                    // Flag pole (right side of screen)
                    for (int p = 0; p < 12; p++) {
                        display_set_pixel(SCREEN_WIDTH - 5, ly + p, COLOR_BUILDING_GRAY);
                    }
                    // Flag (red/white stripes, blue canton)
                    display_fill_rect(SCREEN_WIDTH - 10, ly, 5, 2, COLOR_FLAG_BLUE);
                    display_fill_rect(SCREEN_WIDTH - 10, ly + 2, 8, 1, COLOR_FLAG_RED);
                    display_fill_rect(SCREEN_WIDTH - 10, ly + 3, 8, 1, COLOR_WHITE);
                    display_fill_rect(SCREEN_WIDTH - 10, ly + 4, 8, 1, COLOR_FLAG_RED);
                }
                break;

            case LANDMARK_PALM_LEFT:
                // Palm tree on left side
                if (ly >= 0 && ly < SCREEN_HEIGHT - 10) {
                    // Trunk
                    for (int t = 0; t < 8; t++) {
                        display_set_pixel(3, ly + t, COLOR_PALM_TRUNK);
                    }
                    // Leaves
                    display_set_pixel(2, ly, COLOR_PALM_LEAVES);
                    display_set_pixel(3, ly, COLOR_PALM_LEAVES);
                    display_set_pixel(4, ly, COLOR_PALM_LEAVES);
                    display_set_pixel(1, ly + 1, COLOR_PALM_LEAVES);
                    display_set_pixel(5, ly + 1, COLOR_PALM_LEAVES);
                }
                break;

            case LANDMARK_PALM_RIGHT:
                // Palm tree on right side
                if (ly >= 0 && ly < SCREEN_HEIGHT - 10) {
                    // Trunk
                    for (int t = 0; t < 8; t++) {
                        display_set_pixel(SCREEN_WIDTH - 4, ly + t, COLOR_PALM_TRUNK);
                    }
                    // Leaves
                    display_set_pixel(SCREEN_WIDTH - 5, ly, COLOR_PALM_LEAVES);
                    display_set_pixel(SCREEN_WIDTH - 4, ly, COLOR_PALM_LEAVES);
                    display_set_pixel(SCREEN_WIDTH - 3, ly, COLOR_PALM_LEAVES);
                    display_set_pixel(SCREEN_WIDTH - 6, ly + 1, COLOR_PALM_LEAVES);
                    display_set_pixel(SCREEN_WIDTH - 2, ly + 1, COLOR_PALM_LEAVES);
                }
                break;

            case LANDMARK_CONE:
                // Traffic cone in middle of road
                if (ly >= 0 && ly < SCREEN_HEIGHT - 5) {
                    int cone_x = ROAD_START_X + ROAD_WIDTH / 2;
                    display_set_pixel(cone_x, ly, COLOR_CONE_ORANGE);
                    display_set_pixel(cone_x - 1, ly + 1, COLOR_CONE_ORANGE);
                    display_set_pixel(cone_x, ly + 1, COLOR_CONE_ORANGE);
                    display_set_pixel(cone_x + 1, ly + 1, COLOR_CONE_ORANGE);
                    display_set_pixel(cone_x - 1, ly + 2, COLOR_CONE_ORANGE);
                    display_set_pixel(cone_x, ly + 2, COLOR_CONE_ORANGE);
                    display_set_pixel(cone_x + 1, ly + 2, COLOR_CONE_ORANGE);
                }
                break;
        }
    }
}

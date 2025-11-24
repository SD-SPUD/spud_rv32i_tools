# SpudKit Game Dev Cheat Sheet

Quick reference for the most common APIs used in game development.

## Init

```c
spudkit_init();        // init all peripherals
arcade_init();         // init arcade buttons
rand_seed(value);      // seed random number generator
```

## Display

```c
display_clear(color);                              // clear screen
display_set_pixel(x, y, color);                    // draw single pixel
display_fill_rect(x, y, w, h, color);              // filled rectangle
display_draw_rect(x, y, w, h, color);              // outline rectangle
display_draw_line(x0, y0, x1, y1, color);          // draw line
display_draw_circle(x, y, radius, color);          // draw circle
display_draw_string(x, y, str, fg, bg);            // 8x8 text
display_draw_string_small(x, y, str, fg, bg);      // 4x6 text
display_update();                                  // push buffer to screen
```

## Buttons

```c
arcade_read_all();                                 // read all buttons as bitmask
arcade_button_read(button);                        // read single button (0/1)
```

Button defines: `ARCADE_BUTTON_UP`, `_DOWN`, `_LEFT`, `_RIGHT`, `_A`, `_B`, `_X`, `_Y`, `_START`, `_SELECT`

## Timing

```c
delay_ms(ms);          // delay milliseconds
```

## Utils

```c
rand_range(min, max);  // random number in range
```

## Colors

`COLOR_BLACK`, `COLOR_WHITE`, `COLOR_RED`, `COLOR_GREEN`, `COLOR_BLUE`, `COLOR_YELLOW`, `COLOR_CYAN`, `COLOR_MAGENTA`

## Display Constants

- `DISPLAY_WIDTH` = 64
- `DISPLAY_HEIGHT` = 64

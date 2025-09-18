#include "uart.h"
#include "spudkit.h"

void uart_init(uint32_t baudrate) {
    // TODO: Initialize UART with specified baudrate
    (void)baudrate; // Suppress unused parameter warning
}

void uart_putc(char c) {
    // TODO: Implement UART character output
    volatile uint32_t* uart_tx = (volatile uint32_t*)(SPUD_UART_BASE + 0x04);
    volatile uint32_t* uart_status = (volatile uint32_t*)(SPUD_UART_BASE + 0x08);

    // Wait for TX ready
    while (*uart_status & (1 << 3)); // TXFULL bit
    *uart_tx = c;
}

void uart_puts(const char* str) {
    for(int i = 0; str[i] != '\0'; i++) {
        uart_putc(str[i]);
    }
}

char uart_getc(void) {
    // TODO: Implement UART character input
    volatile uint32_t* uart_rx = (volatile uint32_t*)(SPUD_UART_BASE + 0x00);
    volatile uint32_t* uart_status = (volatile uint32_t*)(SPUD_UART_BASE + 0x08);

    // Wait for RX ready
    while (!(*uart_status & (1 << 0))); // RXVALID bit
    return (char)*uart_rx;
}

int uart_available(void) {
    // TODO: Check if data is available
    volatile uint32_t* uart_status = (volatile uint32_t*)(SPUD_UART_BASE + 0x08);
    return (*uart_status & (1 << 0)) ? 1 : 0; // RXVALID bit
}

void uart_printf(const char* format, ...) {
    // TODO: Implement basic printf functionality
    (void)format; // Suppress unused parameter warning
    uart_puts("[printf not implemented]\n");
}

// UART display utility functions - equivalents of sim_* functions for UART_DISPLAY mode
void uart_put_hex8(uint8_t value) {
    const char hex_chars[] = "0123456789ABCDEF";
    uart_putc(hex_chars[(value >> 4) & 0xF]);
    uart_putc(hex_chars[value & 0xF]);
}

void uart_put_hex16(uint16_t value) {
    uart_put_hex8((value >> 8) & 0xFF);
    uart_put_hex8(value & 0xFF);
}

void uart_put_hex32(uint32_t value) {
    uart_put_hex16((value >> 16) & 0xFFFF);
    uart_put_hex16(value & 0xFFFF);
}

void uart_put_dec(uint32_t value) {
    if (value == 0) {
        uart_putc('0');
        return;
    }

    char buffer[16];
    int i = 0;

    // manual division to avoid library calls
    while (value > 0) {
        uint32_t remainder = 0;
        uint32_t quotient = 0;

        // simple division by 10
        for (int bit = 31; bit >= 0; bit--) {
            remainder = (remainder << 1) | ((value >> bit) & 1);
            if (remainder >= 10) {
                remainder -= 10;
                quotient |= (1U << bit);
            }
        }

        buffer[i++] = '0' + remainder;
        value = quotient;
    }

    // print in reverse order
    for (int j = i - 1; j >= 0; j--) {
        uart_putc(buffer[j]);
    }
}

// terminal display utilities for UART
void uart_cursor_home(void) {
    // ansi escape sequence to move cursor to home position (1,1)
    uart_puts("\033[H");
}

void uart_clear_screen(void) {
    // ansi escape sequence to clear screen
    uart_puts("\033[2J");
    uart_cursor_home();
}

void uart_set_color(uint8_t fg, uint8_t bg) {
    // ansi escape sequence for 256-color mode
    uart_puts("\033[38;5;");
    uart_put_dec(fg);
    uart_puts("m\033[48;5;");
    uart_put_dec(bg);
    uart_puts("m");
}

void uart_reset_color(void) {
    // ansi escape sequence to reset colors
    uart_puts("\033[0m");
}

uint8_t uart_rgb565_to_ansi(uint16_t rgb565) {
    // extract rgb components from rgb565
    uint8_t r = (rgb565 >> 11) & 0x1F;  // 5 bits
    uint8_t g = (rgb565 >> 5) & 0x3F;   // 6 bits
    uint8_t b = rgb565 & 0x1F;          // 5 bits

    // simple mapping to avoid division - use lookup or shift operations
    // map 5-bit values (0-31) to 6-level values (0-5) for color cube
    uint8_t r6 = (r >= 26) ? 5 : (r >= 21) ? 4 : (r >= 16) ? 3 : (r >= 11) ? 2 : (r >= 6) ? 1 : 0;
    uint8_t g6 = (g >= 54) ? 5 : (g >= 43) ? 4 : (g >= 32) ? 3 : (g >= 21) ? 2 : (g >= 11) ? 1 : 0;
    uint8_t b6 = (b >= 26) ? 5 : (b >= 21) ? 4 : (b >= 16) ? 3 : (b >= 11) ? 2 : (b >= 6) ? 1 : 0;

    // for basic colors, use the 6x6x6 color cube
    return 16 + 36 * r6 + 6 * g6 + b6;
}

void uart_exit(int exitcode) {
    uart_puts("\r\n[EXIT: ");
    // Simple hex output of exit code
    uart_putc('0');
    uart_putc('x');
    char hex[] = "0123456789ABCDEF";
    uart_putc(hex[(exitcode >> 4) & 0xF]);
    uart_putc(hex[exitcode & 0xF]);
    uart_puts("]\r\n");

    // Note: On FPGA, we can't actually exit, so just halt
    while(1) {
        __asm__ volatile ("wfi"); // Wait for interrupt (low power)
    }
}
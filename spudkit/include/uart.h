#ifndef SPUDKIT_UART_H
#define SPUDKIT_UART_H

#include <stdint.h>

// UART initialization
void uart_init(uint32_t baudrate);

// Basic UART functions
void uart_putc(char c);
void uart_puts(const char* str);
char uart_getc(void);
int uart_available(void);

// Formatted output (basic printf-like)
void uart_printf(const char* format, ...);

// UART display utility functions (for UART_DISPLAY mode)
void uart_put_hex8(uint8_t value);
void uart_put_hex16(uint16_t value);
void uart_put_hex32(uint32_t value);
void uart_put_dec(uint32_t value);
void uart_cursor_home(void);
void uart_clear_screen(void);
void uart_set_color(uint8_t fg, uint8_t bg);
void uart_reset_color(void);
uint8_t uart_rgb565_to_ansi(uint16_t rgb565);
void uart_exit(int exitcode);

#endif // SPUDKIT_UART_H
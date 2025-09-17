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

#endif // SPUDKIT_UART_H
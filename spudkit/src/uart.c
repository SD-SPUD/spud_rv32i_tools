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
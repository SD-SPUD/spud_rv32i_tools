#include <stdint.h>

// Sim stuff
#define CSR_SIM_CTRL_EXIT (0 << 24)
#define CSR_SIM_CTRL_PUTC (1 << 24)

// UART registers
#define UART_BASE 0x92000000
#define ULITE_RX     (UART_BASE + 0x00)
#define ULITE_TX     (UART_BASE + 0x04)
#define ULITE_STATUS (UART_BASE + 0x08)
#define ULITE_CONTROL (UART_BASE + 0x0C)

// UART status register bits
#define ULITE_STATUS_RXVALID (1 << 0)
#define ULITE_STATUS_RXFULL  (1 << 1)
#define ULITE_STATUS_TXEMPTY (1 << 2)
#define ULITE_STATUS_TXFULL  (1 << 3)
#define ULITE_STATUS_IE      (1 << 4)

// GPIO registers 
#define GPIO_BASE 0x94000000
#define GPIO_DIRECTION  (GPIO_BASE + 0x00)
#define GPIO_INPUT      (GPIO_BASE + 0x04)
#define GPIO_OUTPUT     (GPIO_BASE + 0x08)
#define GPIO_OUTPUT_SET (GPIO_BASE + 0x0C)
#define GPIO_OUTPUT_CLR (GPIO_BASE + 0x10)

static inline void sim_exit(int exitcode)
{
    unsigned int arg = CSR_SIM_CTRL_EXIT | ((unsigned char)exitcode);
    asm volatile ("csrw dscratch,%0": : "r" (arg));
}

static inline void sim_putc(int ch)
{
    unsigned int arg = CSR_SIM_CTRL_PUTC | (ch & 0xFF);
    asm volatile ("csrw dscratch,%0": : "r" (arg));
}

void uart_putc(char c) {
    while (*(volatile uint32_t*)ULITE_STATUS & ULITE_STATUS_TXFULL);
    *(volatile uint32_t*)ULITE_TX = c;
}

void uart_puts(const char* str) {
    for(unsigned int i = 0; str[i] != '\0'; i++) {
        uart_putc(str[i]);
    }
}

void str_put(const char* str){
  for(unsigned int i=0; str[i] != '\0'; i++) {
    sim_putc(str[i]);
  }
}

void led_set(uint32_t led_mask) {
    *(volatile uint32_t*)GPIO_DIRECTION |= led_mask;
    *(volatile uint32_t*)GPIO_OUTPUT_SET = led_mask;
}

void led_clear(uint32_t led_mask) {
    *(volatile uint32_t*)GPIO_DIRECTION |= led_mask;
    *(volatile uint32_t*)GPIO_OUTPUT_CLR = led_mask;
}

// Delay function - adjusted for 100MHz clock
void delay_seconds(uint32_t seconds) {
    volatile uint32_t i, j;
    for(j = 0; j < seconds; j++) {
        for(i = 0; i < 100000000 i++) {
            asm volatile("nop"); // do nothing
        }
    }
}

int main() {
    str_put("hello world!!!\n");
    uart_puts("\r\nhello world!!r\n");
    while(1) {
        led_set(1 << 0);
        uart_puts("LED 0 ON\r\n");
        delay_seconds(1);
        led_clear(1 << 0);
        led_set(1 << 1);
        uart_puts("LED 1 ON, LED 0 OFF\r\n");
        delay_seconds(1);
        // Turn on LED 2, turn off LED 1
        led_clear(1 << 1);
        led_set(1 << 2);
        uart_puts("LED 2 ON, LED 1 OFF\r\n");
        delay_seconds(1);
        led_clear(1 << 2);
        led_set(1 << 3);
        uart_puts("LED 3 ON, LED 2 OFF\r\n");
        delay_seconds(1);
        led_clear(0xF);
        uart_puts("All LEDs OFF\r\n");
    }
    return 0;
}

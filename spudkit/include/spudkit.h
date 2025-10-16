#ifndef SPUDKIT_H
#define SPUDKIT_H

#include <stdint.h>

// main spudkit header that includes all peripheral drivers
#include "display.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "irq.h"
#include "utils.h"
#include "led.h"
#include "ffloat.h"
#include "ffvect3.h"

// spudkit version
#define SPUDKIT_VERSION_MAJOR 1
#define SPUDKIT_VERSION_MINOR 0
#define SPUDKIT_VERSION_PATCH 0

// common memory map definitions
#define SPUD_MEM_BASE       0x80000000
#define SPUD_UART_BASE      0x92000000
#define SPUD_TIMER_BASE     0x91000000
#define SPUD_IRQ_BASE       0x90000000
#define SPUD_SPI_BASE       0x93000000
#define SPUD_GPIO_BASE      0x94000000
#define SPUD_LED_BASE       0x95000000
#define SPUD_DISPLAY_BASE   0x96000000  // future display controller

// simulation control csr definitions
#define CSR_SIM_CTRL_EXIT (0 << 24)
#define CSR_SIM_CTRL_PUTC (1 << 24)

// initialize spudkit library
void spudkit_init(void);

#endif // SPUDKIT_H
#ifndef SPUDKIT_SPI_H
#define SPUDKIT_SPI_H

#include <stdint.h>

// SPI modes
#define SPI_MODE_0 0  // CPOL=0, CPHA=0
#define SPI_MODE_1 1  // CPOL=0, CPHA=1
#define SPI_MODE_2 2  // CPOL=1, CPHA=0
#define SPI_MODE_3 3  // CPOL=1, CPHA=1

// SPI initialization
void spi_init(uint32_t clock_freq, uint8_t mode);

// Basic SPI functions
uint8_t spi_transfer(uint8_t data);
void spi_write(uint8_t data);
uint8_t spi_read(void);

// Bulk transfer functions
void spi_transfer_buffer(uint8_t* tx_buffer, uint8_t* rx_buffer, uint16_t length);
void spi_write_buffer(const uint8_t* buffer, uint16_t length);

// Chip select control
void spi_cs_assert(uint8_t cs_pin);
void spi_cs_deassert(uint8_t cs_pin);

#endif // SPUDKIT_SPI_H
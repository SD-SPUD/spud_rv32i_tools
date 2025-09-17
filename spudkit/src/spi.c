#include "spi.h"
#include "spudkit.h"

void spi_init(uint32_t clock_freq, uint8_t mode) {
    // TODO: Initialize SPI controller
    (void)clock_freq;
    (void)mode;
}

uint8_t spi_transfer(uint8_t data) {
    // TODO: Implement SPI transfer
    volatile uint32_t* spi_tx = (volatile uint32_t*)(SPUD_SPI_BASE + 0x00);
    volatile uint32_t* spi_rx = (volatile uint32_t*)(SPUD_SPI_BASE + 0x04);
    volatile uint32_t* spi_status = (volatile uint32_t*)(SPUD_SPI_BASE + 0x08);

    // Wait for SPI ready
    while (*spi_status & (1 << 0)); // Busy bit

    *spi_tx = data;

    // Wait for transfer complete
    while (*spi_status & (1 << 0)); // Busy bit

    return (uint8_t)*spi_rx;
}

void spi_write(uint8_t data) {
    spi_transfer(data);
}

uint8_t spi_read(void) {
    return spi_transfer(0xFF);
}

void spi_transfer_buffer(uint8_t* tx_buffer, uint8_t* rx_buffer, uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        uint8_t tx_data = tx_buffer ? tx_buffer[i] : 0xFF;
        uint8_t rx_data = spi_transfer(tx_data);
        if(rx_buffer) {
            rx_buffer[i] = rx_data;
        }
    }
}

void spi_write_buffer(const uint8_t* buffer, uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        spi_write(buffer[i]);
    }
}

void spi_cs_assert(uint8_t cs_pin) {
    // TODO: Assert chip select (could use GPIO)
    gpio_write(cs_pin, 0);
}

void spi_cs_deassert(uint8_t cs_pin) {
    // TODO: Deassert chip select (could use GPIO)
    gpio_write(cs_pin, 1);
}
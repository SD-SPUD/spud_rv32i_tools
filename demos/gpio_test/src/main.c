#include <stdint.h>

#define CSR_SIM_CTRL_EXIT (0 << 24)
#define CSR_SIM_CTRL_PUTC (1 << 24)

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

void str_put(const char* str) {
    for(unsigned int i=0; str[i] != '\0'; i++) {
        sim_putc(str[i]);
    }
}

void gpio_write(uint32_t pin, uint32_t value) {
    if (value) {
        *(volatile uint32_t*)GPIO_OUTPUT_SET = (1 << pin);
    } else {
        *(volatile uint32_t*)GPIO_OUTPUT_CLR = (1 << pin);
    }
}

uint32_t gpio_read(uint32_t pin) {
    uint32_t value = *(volatile uint32_t*)GPIO_INPUT;
    return (value >> pin) & 1;
}

void gpio_set_direction(uint32_t pin, uint32_t direction) {
    uint32_t current = *(volatile uint32_t*)GPIO_DIRECTION;
    if (direction) {
        current |= (1 << pin);
    } else {
        current &= ~(1 << pin);
    }
    *(volatile uint32_t*)GPIO_DIRECTION = current;
}

int main() {
    str_put("GPIO Test Demo\n");
    str_put("==============\n\n");
    
    str_put("Setting up GPIO pins...\n");
    gpio_set_direction(0, 1);
    gpio_set_direction(1, 1);
    gpio_set_direction(2, 0);
    gpio_set_direction(3, 0);
    
    str_put("Testing GPIO writes...\n");
    gpio_write(0, 1);
    gpio_write(1, 0);
    str_put("GPIO pin 0 set to HIGH\n");
    str_put("GPIO pin 1 set to LOW\n");
    
    str_put("\nTesting GPIO reads...\n");
    uint32_t pin2_value = gpio_read(2);
    uint32_t pin3_value = gpio_read(3);
    
    str_put("GPIO pin 2 value: ");
    sim_putc('0' + pin2_value);
    str_put("\n");
    
    str_put("GPIO pin 3 value: ");
    sim_putc('0' + pin3_value);
    str_put("\n");
    
    str_put("\nToggling output pins...\n");
    for (int i = 0; i < 5; i++) {
        gpio_write(0, i % 2);
        gpio_write(1, (i + 1) % 2);
        str_put("Toggle ");
        sim_putc('0' + i);
        str_put(" - Pin 0: ");
        sim_putc('0' + (i % 2));
        str_put(", Pin 1: ");
        sim_putc('0' + ((i + 1) % 2));
        str_put("\n");
    }
    
    str_put("\nGPIO test complete!\n");
    return 0;
}
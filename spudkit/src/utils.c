#include "utils.h"
#include "spudkit.h"

// simulation control utilities
void sim_exit(int exitcode) {
    unsigned int arg = CSR_SIM_CTRL_EXIT | ((unsigned char)exitcode);
    asm volatile ("csrw dscratch,%0": : "r" (arg));
}

void sim_putc(int ch) {
    unsigned int arg = CSR_SIM_CTRL_PUTC | (ch & 0xFF);
    asm volatile ("csrw dscratch,%0": : "r" (arg));
}

void sim_puts(const char* str) {
    for(unsigned int i = 0; str[i] != '\0'; i++) {
        sim_putc(str[i]);
    }
}

void sim_put_hex8(uint8_t value) {
    const char hex_chars[] = "0123456789ABCDEF";
    sim_putc(hex_chars[(value >> 4) & 0xF]);
    sim_putc(hex_chars[value & 0xF]);
}

void sim_put_hex16(uint16_t value) {
    sim_put_hex8((value >> 8) & 0xFF);
    sim_put_hex8(value & 0xFF);
}

void sim_put_hex32(uint32_t value) {
    sim_put_hex16((value >> 16) & 0xFFFF);
    sim_put_hex16(value & 0xFFFF);
}

void sim_put_dec(uint32_t value) {
    if (value == 0) {
        sim_putc('0');
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
        sim_putc(buffer[j]);
    }
}

// terminal display utilities
void sim_cursor_home(void) {
    // ansi escape sequence to move cursor to home position (1,1)
    sim_puts("\033[H");
}

void sim_clear_screen(void) {
    // ansi escape sequence to clear screen
    sim_puts("\033[2J");
    sim_cursor_home();
}

void sim_set_color(uint8_t fg, uint8_t bg) {
    // ansi escape sequence for 256-color mode
    sim_puts("\033[38;5;");
    sim_put_dec(fg);
    sim_puts("m\033[48;5;");
    sim_put_dec(bg);
    sim_puts("m");
}

void sim_reset_color(void) {
    // ansi escape sequence to reset colors
    sim_puts("\033[0m");
}

uint8_t sim_rgb565_to_ansi(uint16_t rgb565) {
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

// string utilities
int str_len(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int str_cmp(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

void str_cpy(char* dest, const char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void str_cat(char* dest, const char* src) {
    // find end of dest
    while (*dest) {
        dest++;
    }
    // copy src to end of dest
    str_cpy(dest, src);
}

// math utilities
uint32_t abs_diff(uint32_t a, uint32_t b) {
    return (a > b) ? (a - b) : (b - a);
}

uint32_t min(uint32_t a, uint32_t b) {
    return (a < b) ? a : b;
}

uint32_t max(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

uint32_t clamp(uint32_t value, uint32_t min_val, uint32_t max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// random number generation (simple lfsr)
static uint32_t rand_state = 1;

void rand_seed(uint32_t seed) {
    rand_state = (seed == 0) ? 1 : seed; // avoid 0 state
}

uint32_t rand_next(void) {
    // 32-bit linear feedback shift register
    // taps: 32, 22, 2, 1 (maximal period)
    uint32_t bit = ((rand_state >> 0) ^ (rand_state >> 1) ^ (rand_state >> 21) ^ (rand_state >> 31)) & 1;
    rand_state = (rand_state >> 1) | (bit << 31);
    return rand_state;
}

uint32_t rand_range(uint32_t min_val, uint32_t max_val) {
    if (min_val >= max_val) return min_val;
    uint32_t range = max_val - min_val;

    // for small ranges, use simple bit masking which is much faster
    if (range <= 2) return min_val + (rand_next() & 1);
    if (range <= 4) return min_val + (rand_next() & 3);
    if (range <= 8) return min_val + (rand_next() & 7);
    if (range <= 16) return min_val + (rand_next() & 15);
    if (range <= 32) return min_val + (rand_next() & 31);
    if (range <= 64) return min_val + (rand_next() & 63);

    // for larger ranges, use a simple scaling approach
    // use upper bits to avoid bias for most cases
    uint32_t rand_val = rand_next() >> 16; // use upper 16 bits
    return min_val + (rand_val & (range - 1)); // simple mask for power-of-2ish
}

// bit manipulation utilities
uint8_t count_bits(uint32_t value) {
    uint8_t count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

uint8_t find_first_set(uint32_t value) {
    if (value == 0) return 32; // no bits set

    uint8_t position = 0;
    while (!(value & 1)) {
        value >>= 1;
        position++;
    }
    return position;
}

uint32_t reverse_bits(uint32_t value) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (value & 1);
        value >>= 1;
    }
    return result;
}

// memory utilities
void mem_set(void* ptr, uint8_t value, uint32_t size) {
    volatile uint8_t* byte_ptr = (volatile uint8_t*)ptr;
    for (uint32_t i = 0; i < size; i++) {
        byte_ptr[i] = value;
    }
}

void mem_cpy(void* dest, const void* src, uint32_t size) {
    uint8_t* dest_ptr = (uint8_t*)dest;
    const uint8_t* src_ptr = (const uint8_t*)src;

    for (uint32_t i = 0; i < size; i++) {
        dest_ptr[i] = src_ptr[i];
    }
}

int mem_cmp(const void* ptr1, const void* ptr2, uint32_t size) {
    const uint8_t* p1 = (const uint8_t*)ptr1;
    const uint8_t* p2 = (const uint8_t*)ptr2;

    for (uint32_t i = 0; i < size; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}



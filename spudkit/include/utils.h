#ifndef SPUDKIT_UTILS_H
#define SPUDKIT_UTILS_H

#include <stdint.h>

// simulation control utilities
void sim_exit(int exitcode);
void sim_putc(int ch);
void sim_puts(const char* str);
void sim_put_hex8(uint8_t value);
void sim_put_hex16(uint16_t value);
void sim_put_hex32(uint32_t value);
void sim_put_dec(uint32_t value);

// terminal display utilities
void sim_cursor_home(void);
void sim_clear_screen(void);
void sim_set_color(uint8_t fg, uint8_t bg);
void sim_reset_color(void);
uint8_t sim_rgb565_to_ansi(uint16_t rgb565);

// string utilities
int str_len(const char* str);
int str_cmp(const char* str1, const char* str2);
void str_cpy(char* dest, const char* src);
void str_cat(char* dest, const char* src);

// math utilities
uint32_t abs_diff(uint32_t a, uint32_t b);
uint32_t min(uint32_t a, uint32_t b);
uint32_t max(uint32_t a, uint32_t b);
uint32_t clamp(uint32_t value, uint32_t min_val, uint32_t max_val);

// random number generation (simple lfsr)
void rand_seed(uint32_t seed);
uint32_t rand_next(void);
uint32_t rand_range(uint32_t min_val, uint32_t max_val);

// bit manipulation utilities
uint8_t count_bits(uint32_t value);
uint8_t find_first_set(uint32_t value);
uint32_t reverse_bits(uint32_t value);

// memory utilities
void mem_set(void* ptr, uint8_t value, uint32_t size);
void mem_cpy(void* dest, const void* src, uint32_t size);
int mem_cmp(const void* ptr1, const void* ptr2, uint32_t size);

#endif // SPUDKIT_UTILS_H
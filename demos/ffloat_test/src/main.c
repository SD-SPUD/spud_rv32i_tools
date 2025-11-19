/**
  * FFLoat Test Suite
  */

#include "spudkit.h"

void test_ffmult() {
    uart_puts("\r\n=== FFMULT Tests ===\r\n");

    ffloat a, b, result;

    // Test 1: 2.0 * 3.0 = 6.0
    a = FFLOAT(2.0);
    b = FFLOAT(3.0);
    result = FFMULT(a, b);
    uart_puts("2.0 * 3.0 = ");
    ffprint(result);
    uart_puts(" (expected: 6.0)\r\n");

    // Test 2: 0.5 * 4.0 = 2.0
    a = FFLOAT(0.5);
    b = FFLOAT(4.0);
    result = FFMULT(a, b);
    uart_puts("0.5 * 4.0 = ");
    ffprint(result);
    uart_puts(" (expected: 2.0)\r\n");

    // Test 3: 1.5 * 2.5 = 3.75
    a = FFLOAT(1.5);
    b = FFLOAT(2.5);
    result = FFMULT(a, b);
    uart_puts("1.5 * 2.5 = ");
    ffprint(result);
    uart_puts(" (expected: 3.75)\r\n");

    // Test 4: PI/2 * 2 = PI
    a = FFPIHALF;
    b = FFLOAT(2.0);
    result = FFMULT(a, b);
    uart_puts("PI/2 * 2 = ");
    ffprint(result);
    uart_puts(" (expected: ");
    ffprint(FFPI);
    uart_puts(")\r\n");
}

void test_ffdiv() {
    uart_puts("\r\n=== FFDIV Tests ===\r\n");

    ffloat a, b, result;

    // Test 1: 6.0 / 2.0 = 3.0
    a = FFLOAT(6.0);
    b = FFLOAT(2.0);
    result = FFDIV(a, b);
    uart_puts("6.0 / 2.0 = ");
    ffprint(result);
    uart_puts(" (expected: 3.0)\r\n");

    // Test 2: 5.0 / 4.0 = 1.25
    a = FFLOAT(5.0);
    b = FFLOAT(4.0);
    result = FFDIV(a, b);
    uart_puts("5.0 / 4.0 = ");
    ffprint(result);
    uart_puts(" (expected: 1.25)\r\n");

    // Test 3: PI / PI/2 = 2.0
    a = FFPI;
    b = FFPIHALF;
    result = FFDIV(a, b);
    uart_puts("PI / PI/2 = ");
    ffprint(result);
    uart_puts(" (expected: 2.0)\r\n");

    // Test 4: 6.1994 / 1.570796 (our problem case)
    a = FFLOAT(6.1994);
    b = FFPIHALF;
    result = FFDIV(a, b);
    uart_puts("6.1994 / PI/2 = ");
    ffprint(result);
    uart_puts(" (expected: ~3.946)\r\n");

    // Test 5: Division by 1
    a = FFLOAT(5.5);
    b = FFLOAT(1.0);
    result = FFDIV(a, b);
    uart_puts("5.5 / 1.0 = ");
    ffprint(result);
    uart_puts(" (expected: 5.5)\r\n");

    // Test 6: Very small divisor
    uart_puts("Starting Test 6...\r\n");
    a = FFLOAT(1.0);
    b = FFLOAT(0.01);
    uart_puts("a = ");
    ffprint(a);
    uart_puts(", b = ");
    ffprint(b);
    uart_puts("\r\n");
    result = FFDIV(a, b);
    uart_puts("1.0 / 0.01 = ");
    ffprint(result);
    uart_puts(" (expected: 100.0)\r\n");

    // Test 7: Large number division
    uart_puts("Starting Test 7...\r\n");
    a = FFLOAT(1000.0);
    b = FFLOAT(3.0);
    uart_puts("a = ");
    ffprint(a);
    uart_puts(", b = ");
    ffprint(b);
    uart_puts("\r\n");
    result = FFDIV(a, b);
    uart_puts("1000.0 / 3.0 = ");
    ffprint(result);
    uart_puts(" (expected: 333.333)\r\n");

    // Test 8: Division by very small number
    uart_puts("Starting Test 8...\r\n");
    a = FFLOAT(2.0);
    b = FFLOAT(0.001);
    uart_puts("a = ");
    ffprint(a);
    uart_puts(", b = ");
    ffprint(b);
    uart_puts("\r\n");
    uart_puts("About to call FFDIV...\r\n");
    result = FFDIV(a, b);
    uart_puts("2.0 / 0.001 = ");
    ffprint(result);
    uart_puts(" (expected: 2000.0)\r\n");

    // Test 9: Very large dividend (test overflow case)
    uart_puts("Starting Test 9...\r\n");
    a = FFLOAT(50000.0);
    b = FFLOAT(7.0);
    uart_puts("a = ");
    ffprint(a);
    uart_puts(", b = ");
    ffprint(b);
    uart_puts("\r\n");
    uart_puts("About to call FFDIV...\r\n");
    result = FFDIV(a, b);
    uart_puts("50000.0 / 7.0 = ");
    ffprint(result);
    uart_puts(" (expected: 7142.857)\r\n");

    // Test 10: Division by zero
    uart_puts("Starting Test 10...\r\n");
    a = FFLOAT(5.0);
    b = FFLOAT(0.0);
    result = FFDIV(a, b);
    uart_puts("5.0 / 0.0 = ");
    ffprint(result);
    uart_puts(" (expected: 0.0)\r\n");

    // Test 11: Maximum positive number
    uart_puts("Starting Test 11...\r\n");
    a = INT32_MAX;
    b = FFLOAT(2.0);
    uart_puts("About to call FFDIV...\r\n");
    result = FFDIV(a, b);
    uart_puts("MAX_INT / 2.0 = ");
    ffprint(result);
    uart_puts("\r\n");

    // Test 12: Tiny divided by tiny
    uart_puts("Starting Test 12...\r\n");
    a = FFLOAT(0.0001);
    b = FFLOAT(0.0001);
    result = FFDIV(a, b);
    uart_puts("0.0001 / 0.0001 = ");
    ffprint(result);
    uart_puts(" (expected: 1.0)\r\n");

    // Test 13: Huge divided by tiny
    uart_puts("Starting Test 13...\r\n");
    a = FFLOAT(100000.0);
    b = FFLOAT(0.00001);
    uart_puts("About to call FFDIV...\r\n");
    result = FFDIV(a, b);
    uart_puts("100000.0 / 0.00001 = ");
    ffprint(result);
    uart_puts(" (expected: huge number)\r\n");

    // Test 14: Nearly zero divisor
    uart_puts("Starting Test 14...\r\n");
    a = FFLOAT(1.0);
    b = 1; // Smallest possible non-zero ffloat
    uart_puts("About to call FFDIV...\r\n");
    result = FFDIV(a, b);
    uart_puts("1.0 / tiny = ");
    ffprint(result);
    uart_puts("\r\n");

    // Test 15: Negative numbers
    uart_puts("Starting Test 15...\r\n");
    a = FFLOAT(-10.0);
    b = FFLOAT(3.0);
    result = FFDIV(a, b);
    uart_puts("-10.0 / 3.0 = ");
    ffprint(result);
    uart_puts(" (expected: -3.333)\r\n");

    // Test 16: Negative divided by negative
    uart_puts("Starting Test 16...\r\n");
    a = FFLOAT(-20.0);
    b = FFLOAT(-4.0);
    result = FFDIV(a, b);
    uart_puts("-20.0 / -4.0 = ");
    ffprint(result);
    uart_puts(" (expected: 5.0)\r\n");

    // Test 17: Positive divided by negative
    uart_puts("Starting Test 17...\r\n");
    a = FFLOAT(15.0);
    b = FFLOAT(-3.0);
    result = FFDIV(a, b);
    uart_puts("15.0 / -3.0 = ");
    ffprint(result);
    uart_puts(" (expected: -5.0)\r\n");

    // Test 18: Very negative numbers
    uart_puts("Starting Test 18...\r\n");
    a = FFLOAT(-1000.0);
    b = FFLOAT(-0.001);
    uart_puts("About to call FFDIV...\r\n");
    result = FFDIV(a, b);
    uart_puts("-1000.0 / -0.001 = ");
    ffprint(result);
    uart_puts(" (expected: 1000000.0)\r\n");

    // Test 19: Minimum integer
    uart_puts("Starting Test 19...\r\n");
    a = INT32_MIN;
    b = FFLOAT(-1.0);
    uart_puts("About to call FFDIV...\r\n");
    result = FFDIV(a, b);
    uart_puts("MIN_INT / -1.0 = ");
    ffprint(result);
    uart_puts(" (expected: huge positive)\r\n");
}

void test_ffmod() {
    uart_puts("\r\n=== FFMOD Tests ===\r\n");

    ffloat a, b, result;

    // Test 1: 7.0 mod 3.0 = 1.0
    a = FFLOAT(7.0);
    b = FFLOAT(3.0);
    result = FFMOD(a, b);
    uart_puts("7.0 mod 3.0 = ");
    ffprint(result);
    uart_puts(" (expected: 1.0)\r\n");

    // Test 2: 5.5 mod 2.0 = 1.5
    a = FFLOAT(5.5);
    b = FFLOAT(2.0);
    result = FFMOD(a, b);
    uart_puts("5.5 mod 2.0 = ");
    ffprint(result);
    uart_puts(" (expected: 1.5)\r\n");

    // Test 3: PI mod PI/2
    a = FFPI;
    b = FFPIHALF;
    result = FFMOD(a, b);
    uart_puts("PI mod PI/2 = ");
    ffprint(result);
    uart_puts(" (expected: 0.0)\r\n");

    // Test 4: 6.1994 mod PI/2 (our problem case)
    a = FFLOAT(6.1994);
    b = FFPIHALF;
    result = FFMOD(a, b);
    uart_puts("6.1994 mod PI/2 = ");
    ffprint(result);
    uart_puts(" (expected: ~0.487)\r\n");

    // Test 5: 1.0 mod 2.0 = 1.0
    a = FFLOAT(1.0);
    b = FFLOAT(2.0);
    result = FFMOD(a, b);
    uart_puts("1.0 mod 2.0 = ");
    ffprint(result);
    uart_puts(" (expected: 1.0)\r\n");

    // Test 6: 2*PI mod PI/2
    a = FF2PI;
    b = FFPIHALF;
    result = FFMOD(a, b);
    uart_puts("2*PI mod PI/2 = ");
    ffprint(result);
    uart_puts(" (expected: 0.0)\r\n");
}

void show_constants() {
    uart_puts("\r\n=== FFLoat Constants ===\r\n");
    uart_puts("PI/2 = ");
    ffprint(FFPIHALF);
    uart_puts("\r\nPI = ");
    ffprint(FFPI);
    uart_puts("\r\n2*PI = ");
    ffprint(FF2PI);
    uart_puts("\r\n");
}

int ffloat_test_main() {
    spudkit_init();
    uart_puts("\r\nFFLoat Test Suite\r\n");
    uart_puts("==================\r\n");

    show_constants();
    test_ffmult();
    test_ffdiv();
    test_ffmod();

    uart_puts("\r\n=== Test Complete ===\r\n");

    while(1) {
        delay_ms(1000);
    }

    return 0;
}

int main() {
    return ffloat_test_main();
}

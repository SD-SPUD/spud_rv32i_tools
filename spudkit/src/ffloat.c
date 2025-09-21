#include "ffloat.h"
#include "uart.h"
/**
  * FAKE FLOAT or FIXED FLOAT
  * we use a mantisa to do operations with the float
  * use with caution as operations are computationaly are expensive
  */

void ffprint(ffloat value) {
    // handle negative values
    if (value < 0) {
        sim_putc('-');
        uart_putc('-');
        value = -value;
    }

    // extract integer part
    int32_t int_part = value >> MANTISA;

    // extract fractional part
    uint32_t frac_part = value & ((1 << MANTISA) - 1);

    // convert fractional to decimal (multiply by 10000 for 4 decimal places)
    uint32_t decimal = (frac_part * 10000) / (1 << MANTISA);

    // print integer.decimal with padding to both sim and uart
    sim_put_dec(int_part);
    uart_put_dec(int_part);

    sim_putc('.');
    uart_putc('.');

    // pad with zeros if needed
    if (decimal < 1000) {
        sim_putc('0');
        uart_putc('0');
    }
    if (decimal < 100) {
        sim_putc('0');
        uart_putc('0');
    }
    if (decimal < 10) {
        sim_putc('0');
        uart_putc('0');
    }

    sim_put_dec(decimal);
    uart_put_dec(decimal);
}

/*
 * to calculate sin we can  approximate using a taylor series.
 * taylor series expansion: x - x^3/3! + x^5/5! - x^7/7! + ...
 * we can precalculate some constants and get a pretty fast sin
 */
ffloat ffsin(ffloat x) {
    // normalize to [0, 2*pi] range
    ffloat two_pi = FF2PI;
    if (x < 0) {
        int32_t k = FFDIV(-x, two_pi) + 1;
        x += FFMULT(k, two_pi);
    }
    if (x >= two_pi) {
        int32_t k = FFDIV(x, two_pi);
        x -= FFMULT(k, two_pi);
    }

    // determine quadrant and adjust for correct sign
    int negate = 0;
    if (x >= FFPI) {
        x -= FFPI;
        negate = 1;
    }
    if (x > FFDIV(FFPI, FFLOAT(2))) {
        x = FFPI - x;
    }

    // taylor series constants (1/factorial)
    const ffloat INV_FACT3 = FFDIV(FFLOAT(1), FFLOAT(6));
    const ffloat INV_FACT5 = FFDIV(FFLOAT(1), FFLOAT(120));
    const ffloat INV_FACT7 = FFDIV(FFLOAT(1), FFLOAT(5040));
    // compute powers of reduced argument
    ffloat x2 = FFMULT(x, x);
    ffloat x3 = FFMULT(x2, x);
    ffloat x5 = FFMULT(x3, x2);
    ffloat x7 = FFMULT(x5, x2);
    // taylor series: x - x^3/3! + x^5/5! - x^7/7!
    ffloat result = x - FFMULT(x3, INV_FACT3) + FFMULT(x5, INV_FACT5) - FFMULT(x7, INV_FACT7);
return negate ? -result : result;
}

ffloat ffcos(ffloat x) {
    return ffsin(x - FFPIHALF);
}

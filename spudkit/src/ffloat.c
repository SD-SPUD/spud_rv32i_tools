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
    // precalculate xxx
    ffloat k = FFDIV(FFINT(x * FFLOAT(2.0)), FFPI);

    // basically get mod 2/pi
    x = FFMULT(FFMULT(x - k, FFPI), FFLOAT(0.5));

    // taylor series constants (1/factorial)
    const ffloat INV_FACT3 = FFDIV(FFLOAT(1), FFLOAT(6));
    const ffloat INV_FACT5 = FFDIV(FFLOAT(1), FFLOAT(120));
    const ffloat INV_FACT7 = FFDIV(FFLOAT(1), FFLOAT(5040));

    // get the quadent
    int q = k % 4;
    switch (q) {
    case 0: x = x; break;
    case 1: x = FFMULT(FFPI, FFLOAT(0.5)) - x; break;
    case 2: x = -x; break;
    case 3: x = -(FFMULT(FFPI, FFLOAT(0.5)) - x); break;
    }

    // compute powers of reduced argument
    ffloat x2 = FFMULT(x, x);
    ffloat x3 = FFMULT(x2, x);
    ffloat x5 = FFMULT(x3, x2);
    ffloat x7 = FFMULT(x5, x2);

    // taylor series: x - x^3/3! + x^5/5! - x^7/7!
    ffloat approx = x
                  - FFMULT(x3, INV_FACT3)
                  + FFMULT(x5, INV_FACT5)
                  - FFMULT(x7, INV_FACT7);


    return approx;
}

/** to calculate cos we can approximate using a Taylor series.
 * taylor series expansion: 1 - x^2/2! + x^4/4! - x^6/6! + ...
 * this is analogous to the sin implementation, but with its own series and normalization
 */
ffloat ffcos(ffloat x) {
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

    // quadrant reduction and sign correction for cosine
    int negate = 0;
    if (x >= FFPI) {
        x -= FFPI;
        negate = 1;
    }
    if (x > FFDIV(FFPI, FFLOAT(2))) {
        x = FFPI - x;
    }

    // taylor series constants for cosine
    const ffloat INV_FACT2 = FFDIV(FFLOAT(1), FFLOAT(2));
    const ffloat INV_FACT4 = FFDIV(FFLOAT(1), FFLOAT(24));
    const ffloat INV_FACT6 = FFDIV(FFLOAT(1), FFLOAT(720));

    // powers
    ffloat x2 = FFMULT(x, x);
    ffloat x4 = FFMULT(x2, x2);
    ffloat x6 = FFMULT(x4, x2);

    // taylor series: 1 - x^2/2! + x^4/4! - x^6/6!
    ffloat result = FFLOAT(1)
                  - FFMULT(x2, INV_FACT2)
                  + FFMULT(x4, INV_FACT4)
                  - FFMULT(x6, INV_FACT6);

    return negate ? -result : result;
}


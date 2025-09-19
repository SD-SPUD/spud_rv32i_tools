#ifndef SPUDKIT_FFLOAT
#define SPUDKIT_FFLOAT

#include "utils.h"

// this is where the fractional bits are defined
#define MANTISA 16
#define FFPI FFLOAT(3.14159265359)
typedef int32_t ffloat;
// ffloat creation and operation macros
#define FFLOAT(x) ((ffloat)((x) * (1 << MANTISA)))
#define FFMULT(a, b) (((a) * (b)) >> MANTISA)
#define FFDIV(a, b) FAST_DIV(a, b, MANTISA)
ffloat ffsin(ffloat theta);
ffloat ffcos(ffloat theta);
void ffprint(ffloat value);
#endif

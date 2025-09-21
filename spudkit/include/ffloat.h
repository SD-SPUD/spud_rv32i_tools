#ifndef SPUDKIT_FFLOAT
#define SPUDKIT_FFLOAT

#include "utils.h"

typedef int32_t ffloat;
// ffloat creation and operation macros
#define MANTISA 16
#define FFLOAT(x) ((ffloat)((x) * (1 << MANTISA)))
#define FFMULT(a, b) ({ \
    ffloat _a = (a); \
    ffloat _b = (b); \
    ffloat _result = ((_a >> (MANTISA/2)) * (_b >> (MANTISA/2))); \
    _result; \
})
#define FFDIV(a, b) ({ \
    ffloat _a = (a); \
    ffloat _b = (b); \
    ffloat _div = (_b >> MANTISA); \
    ffloat _result = (_div == 0) ? 0 : _a / _div; \
    _result; \
})
#define FFINT(x) ((x) >> MANTISA)
#define FFPIHALF FFLOAT(1.570796)
#define FFPI FFLOAT(3.14159)
#define FF2PI FFLOAT(6.28318)

ffloat ffsin(ffloat theta);
ffloat ffcos(ffloat theta);
void ffprint(ffloat value);

#endif

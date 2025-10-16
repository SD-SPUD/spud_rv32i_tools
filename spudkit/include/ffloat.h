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
    ffloat _result; \
    if (_b == 0) { \
        _result = 0; \
    } else if (_a > (INT32_MAX >> MANTISA) || _b < (1 << (MANTISA/2))) { \
        _result = (_a / ((_b >> (MANTISA/2)) | 1)) << (MANTISA/2); \
    } else { \
        _result = (_a << MANTISA) / _b; \
    } \
    _result; \
})
#define FFMOD(a, b) ({ \
    ffloat _a = (a); \
    ffloat _b = (b); \
    ffloat _result = (_b == 0) ? 0 : (_a % _b); \
    _result; \
})
#define FFINT(x) ((x) >> MANTISA)
#define FFPIQUARTER FFLOAT(0.7853982)
#define FFPIHALF FFLOAT(1.570796)
#define FFPI FFLOAT(3.14159)
#define FF2PI FFLOAT(6.28318)

ffloat ffsin(ffloat theta);
ffloat ffcos(ffloat theta);
void ffprint(ffloat value);

#endif

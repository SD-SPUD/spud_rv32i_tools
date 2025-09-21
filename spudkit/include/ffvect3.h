#ifndef SPUDKIT_FFVECT3
#define SPUDKIT_FFVECT3

#include "ffloat.h"

// fixed float 3d vector structure
struct ffvect3_t {
    ffloat x;
    ffloat y;
    ffloat z;
};

// function declarations
struct ffvect3_t ffvect3_mult(struct ffvect3_t vect3, struct ffvect3_t matrix[3]);
void ffvect3_print(struct ffvect3_t vect);

// makes a rotation matrix for x axis
#define RX(theta) \
    ((struct ffvect3_t[3]) { \
    {FFLOAT(1),    FFLOAT(0),     FFLOAT(0)}, \
    {FFLOAT(0), ffcos(theta), -ffsin(theta)}, \
    {FFLOAT(0), ffsin(theta),  ffcos(theta)}, \
    })

// makes a rotation matrix for y axis
#define RY(theta) \
    ((struct ffvect3_t[3]) { \
    { ffcos(theta), FFLOAT(0), ffsin(theta)}, \
    {    FFLOAT(0), FFLOAT(1),    FFLOAT(0)}, \
    {-ffsin(theta), FFLOAT(0), ffcos(theta)}, \
    })

// makes a rotation matrix for z axis
#define RZ(theta) \
    ((struct ffvect3_t[3]) { \
    {ffcos(theta), -ffsin(theta), FFLOAT(0)}, \
    {ffsin(theta),  ffcos(theta), FFLOAT(0)}, \
    {   FFLOAT(0),     FFLOAT(0), FFLOAT(1)}, \
    })

// rotation macros
#define ROTATEX_VECT3(vect, theta) ffvect3_mult(vect, RX(theta))
#define ROTATEY_VECT3(vect, theta) ffvect3_mult(vect, RY(theta))
#define ROTATEZ_VECT3(vect, theta) ffvect3_mult(vect, RZ(theta))

#endif

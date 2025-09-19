#ifndef SPUDKIT_FFVECT3
#define SPUDKIT_FFVECT3

// makes a rotation matrix for x axis
#define RX(theta) \
    { \
    struct ffvect3_t{1,            0,             0}, \
    struct ffvect3_t{0, ffcos(theta), -ffsin(theta)}, \
    struct ffvect3_t{0, ffsin(theta),  ffcos(theta)}, \
    }

// makes a rotation matrix for y axis
#define RY(theta) \
    { \
    struct ffvect3_t{ ffcos(theta), 0, ffsin(theta)}, \
    struct ffvect3_t{            0, 1,            0}, \
    struct ffvect3_t{-ffsin(theta), 0, ffcos(theta)}, \
    }

// makes a rotation matrix for z axis
#define RZ(theta) \
    { \
    struct ffvect3_t{ffcos(theta), -ffsin(theta), 0}, \
    struct ffvect3_t{ffsin(theta),  ffcos(theta), 0}, \
    struct ffvect3_t{           0,             0, 1}, \
    }

// rotation macros
#define ROTATEX_VECT3(vect, theta) ffvect3_mult(vect, RX(theta))
#define ROTATEY_VECT3(vect, theta) ffvect3_mult(vect, RY(theta))
#define ROTATEZ_VECT3(vect, theta) ffvect3_mult(vect, RZ(theta))

#endif

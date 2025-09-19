#include "ffloat.h"
#include "ffvect3.h"

// fixed float 3 point vector
struct ffvect3_t {
    ffloat x,
    ffloat y,
    ffloat z,
};

// pass in a 3d matrix
struct ffvect3_t ffvect3_mult (struct ffvect3_t vect3, struct ffvect3_t matrix[3]) {
    struct ffvect3_t mult = {0, 0, 0};
    mult.x += FFMULT(vect3.x, matrix[0].x);
    mult.x += FFMULT(vect3.y, matrix[0].y);
    mult.x += FFMULT(vect3.z, matrix[0].z);
    mult.y += FFMULT(vect3.x, matrix[1].x);
    mult.y += FFMULT(vect3.y, matrix[1].y);
    mult.y += FFMULT(vect3.z, matrix[1].z);
    mult.z += FFMULT(vect3.x, matrix[2].x);
    mult.z += FFMULT(vect3.y, matrix[2].y);
    mult.z += FFMULT(vect3.z, matrix[2].z);
    return mult;
}


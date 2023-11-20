#ifndef VEC_SUM_H
#define VEC_SUM_H

#ifndef UNROLL_TYPE
    #define UNROLL_TYPE (0)
#endif

#include <stdint.h>

// Returns the sum of 'length' many array elements
// array: vector with 'length' float values
// length: length of the vector
float vec_sum(float *array, uint64_t length);

#endif

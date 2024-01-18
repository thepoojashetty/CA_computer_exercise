#ifndef JACOBI_H
#define JACOBI_H

#ifndef Bx
    #define Bx (1000)
#endif

#ifndef By
    #define By (1000)
#endif

#include <stdint.h>

/* This function iterates over a grid to perform a jacobi time step.
 */
void jacobi_subgrid(double* grid_source, double* grid_target, uint32_t x, uint32_t y);

#endif

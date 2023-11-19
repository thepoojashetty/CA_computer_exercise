#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void jacobi(double* grid_source, double* grid_target, uint32_t x, uint32_t y) {
	// TODO implement
	for(uint32_t i=1;i<y-1;i++){
		for(uint32_t j=1;j<x-1;j++){
			grid_target[i*x+j] = (grid_source[(i-1)*x+j]
								+ grid_source[i*x+j+1]
								+ grid_source[i*x+j-1]
								+ grid_source[(i+1)*x+j])/4;
		}
	}
}

#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <immintrin.h>

void jacobi(double* grid_source, double* grid_target, uint32_t x, uint32_t y) {
	// TODO implement
	#ifdef JACOBI_COL
	//columnwise
	for(uint32_t j=1;j<x-1;j++){
		#pragma novector
		#pragma nounroll
		for(uint32_t i=1;i<y-1;i++){
			grid_target[i*x+j] = (grid_source[(i-1)*x+j]
								+ grid_source[i*x+j+1]
								+ grid_source[i*x+j-1]
								+ grid_source[(i+1)*x+j])*0.25;
		}
	}
	#else
	//rowwise
	for(uint32_t i=1;i<y-1;i++){
		#pragma novector
		#pragma nounroll
		for(uint32_t j=1;j<x-1;j++){
			grid_target[i*x+j] = (grid_source[(i-1)*x+j]
								+ grid_source[i*x+j+1]
								+ grid_source[i*x+j-1]
								+ grid_source[(i+1)*x+j])*0.25;
		}
	}
	#endif
}

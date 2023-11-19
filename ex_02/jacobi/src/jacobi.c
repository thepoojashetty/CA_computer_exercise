#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void jacobi(double* grid_source, double* grid_target, uint32_t x, uint32_t y) {
	// TODO implement
	for(uint32_t i=1;i<y-1;i++){
		//no unrolling
		#if JACOBIUNROLLTYPE == 1
		#pragma novector
		#pragma nounroll
		for(uint32_t j=1;j<x-1;j++){
			grid_target[i*x+j] = (grid_source[(i-1)*x+j]
								+ grid_source[i*x+j+1]
								+ grid_source[i*x+j-1]
								+ grid_source[(i+1)*x+j])*0.25;
		}

		//Twofold unrolling
		#elif JACOBIUNROLLTYPE == 2
		uint32_t remainder = (x-1-1)%2;
		#pragma novector
		#pragma nounroll
		for(uint32_t j=1;j<x-1-remainder;j+=2){
			grid_target[i*x+j] = (grid_source[(i-1)*x+j]
								+ grid_source[i*x+j+1]
								+ grid_source[i*x+j-1]
								+ grid_source[(i+1)*x+j])*0.25;
			grid_target[i*x+j+1] = (grid_source[(i-1)*x+j+1]
								+ grid_source[i*x+j+2]
								+ grid_source[i*x+j]
								+ grid_source[(i+1)*x+j+1])*0.25;
		}
		if(remainder){
			grid_target[i*x+(x-1-remainder)] = (grid_source[(i-1)*x+(x-1-remainder)]
								+ grid_source[i*x+(x-1-remainder)+1]
								+ grid_source[i*x+(x-1-remainder)-1]
								+ grid_source[(i+1)*x+(x-1-remainder)])*0.25;
		}

		//Fourfold unrolling
		#elif JACOBIUNROLLTYPE == 4
		uint32_t remainder = (x-1-1)%4;
		#pragma novector
		#pragma nounroll
		for(uint32_t j=1;j<x-1-remainder;j+=4){
			grid_target[i*x+j] = (grid_source[(i-1)*x+j]
								+ grid_source[i*x+j+1]
								+ grid_source[i*x+j-1]
								+ grid_source[(i+1)*x+j])*0.25;
			grid_target[i*x+j+1] = (grid_source[(i-1)*x+j+1]
								+ grid_source[i*x+j+2]
								+ grid_source[i*x+j]
								+ grid_source[(i+1)*x+j+1])*0.25;
			grid_target[i*x+j+2] = (grid_source[(i-1)*x+j+2]
								+ grid_source[i*x+j+3]
								+ grid_source[i*x+j+1]
								+ grid_source[(i+1)*x+j+2])*0.25;
			grid_target[i*x+j+3] = (grid_source[(i-1)*x+j+3]
								+ grid_source[i*x+j+4]
								+ grid_source[i*x+j+2]
								+ grid_source[(i+1)*x+j+3])*0.25;
		}
		for(uint32_t k=remainder;k>0;k--){
			grid_target[i*x+(x-1-k)] = (grid_source[(i-1)*x+(x-1-k)]
								+ grid_source[i*x+(x-1-k)+1]
								+ grid_source[i*x+(x-1-k)-1]
								+ grid_source[(i+1)*x+(x-1-k)])*0.25;
		}
		//normal execution
		#else
		for(uint32_t j=1;j<x-1;j++){
			grid_target[i*x+j] = (grid_source[(i-1)*x+j]
								+ grid_source[i*x+j+1]
								+ grid_source[i*x+j-1]
								+ grid_source[(i+1)*x+j])*0.25;
		}
		#endif
	}
}

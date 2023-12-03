#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <immintrin.h>

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
		__m128d accu[4],tmp1,tmp2,res,div_factor=__mm_set_pd(0.25,0.25,0.25,0.25);
		#pragma novector
		#pragma unroll(2)
		for(uint32_t j=1;j<x-1-remainder;j+=2){
			accu[0] = _mm_loadu_pd(&grid_source[(i-1)*x+j]);//bottom
			accu[1] = _mm_loadu_pd(&grid_source[i*x+j+1]);//right
			accu[2] = _mm_loadu_pd(&grid_source[i*x+j-1]);//left
			accu[3] = _mm_loadu_pd(&grid_source[(i+1)*x+j]);//top

			tmp1=_mm_add_pd(accu[0],accu[1]);
			tmp2=_mm_add_pd(accu[2],accu[3]);
			res=_mm_add_pd(tmp1,tmp2);
			//res=_mm_mul_pd(res,_mm_set1_pd(0.25));
			res=_mm_mul_pd(res,div_factor);
			_mm_storeu_pd(&grid_target[i*x+j],res);
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
		__m256d accu[4],res,tmp1,tmp2,div_factor=__mm256_set_pd(0.25,0.25,0.25,0.25);
		#pragma novector
		#pragma unroll(4)
		for(uint32_t j=1;j<x-1-remainder;j+=4){
			accu[0] = _mm256_loadu_pd(&grid_source[(i-1)*x+j]);//bottom
			accu[1] = _mm256_loadu_pd(&grid_source[i*x+j+1]);//right
			accu[2] = _mm256_loadu_pd(&grid_source[i*x+j-1]);//left
			accu[3] = _mm256_loadu_pd(&grid_source[(i+1)*x+j]);//top

			tmp1=_mm256_add_pd(accu[0],accu[1]);
			tmp2=_mm256_add_pd(accu[2],accu[3]);
			res=_mm256_add_pd(tmp1,tmp2);
			res=_mm256_mul_pd(res,div_factor);
			_mm256_storeu_pd(&grid_target[i*x+j],res);
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

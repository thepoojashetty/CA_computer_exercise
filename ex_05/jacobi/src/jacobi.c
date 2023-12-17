#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <immintrin.h>

void jacobi(double* grid_source, double* grid_target, uint32_t x, uint32_t y) {
	// TODO implement
	//Fourfold unrolling using AVX

	//Using cache blocking
	#ifdef CACHE_BLOCKING
	__m256d accu[4],res,tmp1,tmp2,div_factor=_mm256_set_pd(0.25,0.25,0.25,0.25);
	uint32_t num_block = (x-1)/Bx;
	uint32_t block_start = 1;
	for(uint32_t b=0;b<num_block;b++){ //for the block
		uint32_t block_end = block_start+Bx;
		for(uint32_t i=1;i<y-1;i++){ //for the row (along y)
			#pragma novector
			#pragma unroll(4)
			for(uint32_t j=block_start;j<block_end;j+=4){
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
		}
		block_start = block_end;
	}
	//last block
	uint32_t last_blk_remainder = (x-block_start-1)%4;//x-1+block_start+1
	for(uint32_t i=1;i<y-1;i++){ //for the row (along y)
		#pragma novector
		#pragma unroll(4)
		for(uint32_t j=block_start;j<x-1-last_blk_remainder;j+=4){
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
		for(uint32_t k=last_blk_remainder;k>0;k--){
			grid_target[i*x+(x-1-k)] = (grid_source[(i-1)*x+(x-1-k)]
								+ grid_source[i*x+(x-1-k)+1]
								+ grid_source[i*x+(x-1-k)-1]
								+ grid_source[(i+1)*x+(x-1-k)])*0.25;
		}
	}

	//without cache blocking
	#else
	for(uint32_t i=1;i<y-1;i++){
		uint32_t remainder = (x-1-1)%4;
		__m256d accu[4],res,tmp1,tmp2,div_factor=_mm256_set_pd(0.25,0.25,0.25,0.25);
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
	}
	#endif
}

#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <immintrin.h>

void jacobi(double* grid_source, double* grid_target, uint32_t x, uint32_t y) {
	// TODO implement
	//Fourfold unrolling using AVX
	//Using cache blocking along both axes
	__m256d accu[4],res,tmp1,tmp2,div_factor=_mm256_set_pd(0.25,0.25,0.25,0.25);
	uint32_t num_block_x = (x-1)/Bx;
	uint32_t num_block_y = (y-1)/By;
	uint32_t block_start_x = 1;
	uint32_t block_start_y = 1;
	uint32_t block_end_y=block_start_y+By,block_end_x;
	uint32_t last_blk_remainder_x = (((x-1)%Bx)-1)%4;

	if(num_block_y==0){
		block_end_y = y-1;
	}

	uint32_t by=0;
	while(block_start_y < y-1){
		block_start_x=1;
		for(uint32_t bx=0;bx<num_block_x;bx++){
			block_end_x = block_start_x+Bx;
			for(uint32_t i=block_start_y;i<block_end_y;i++){ //for the row (along y)
				#pragma novector
				#pragma unroll(4)
				for(uint32_t j=block_start_x;j<block_end_x;j+=4){
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
			block_start_x = block_end_x;
		}
		//last block along x
		for(uint32_t i=block_start_y;i<block_end_y;i++){ //for the row (along y)
			#pragma novector
			#pragma unroll(4)
			for(uint32_t j=block_start_x;j<x-1-last_blk_remainder_x;j+=4){
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
			for(uint32_t k=last_blk_remainder_x;k>0;k--){
				grid_target[i*x+(x-1-k)] = (grid_source[(i-1)*x+(x-1-k)]
									+ grid_source[i*x+(x-1-k)+1]
									+ grid_source[i*x+(x-1-k)-1]
									+ grid_source[(i+1)*x+(x-1-k)])*0.25;
			}
		}

		by++;
		block_start_y = block_end_y;
		if(by<num_block_y){
			block_end_y = block_start_y+By;
		}else{
			block_end_y = y-1;
		}
	}

}

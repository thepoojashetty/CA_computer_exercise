#include "vec_sum.h"

float vec_sum(float *array, uint64_t length) {
	float sum_m[1] = {0.0f};

	#if UNROLL_TYPE == 1
	float sum = 0.0f;
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length;i++){
		sum = sum + array[i];
	}
	sum_m[0] = sum;

	#elif UNROLL_TYPE == 2
	float sum[2] = {0.0f,0.0f};
	uint32_t rem = length % 2;
	
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length-rem;i+=2){
		sum[0]+= array[i];
		sum[1]+= array[i+1];
	}
	sum_m[0] = sum[0] + sum[1];
	if(rem){
		sum_m[0]+=array[length-rem];
	}

	#elif UNROLL_TYPE == 3
	float sum[3] = {0.0f,0.0f,0.0f};
	uint32_t rem = length % 3;
	
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length-rem;i+=3){
		sum[0]+= array[i];
		sum[1]+= array[i+1];
		sum[2]+= array[i+2];
	}
	for(int i=0;i<3;i++){
		sum_m[0]+=sum[i];
	}
	while(rem>0){
		sum_m[0]+=array[length-rem];
		rem--;
	}

	#elif UNROLL_TYPE == 4
	float sum[4] = {0.0f,0.0f,0.0f,0.0f};
	uint32_t rem = length % 4;
	
	
	#pragma nounroll
	#pragma novector
	
	for(int i=0;i<length-rem;i+=4){
		sum[0]+= array[i];
		sum[1]+= array[i+1];
		sum[2]+= array[i+2];
		sum[3]+= array[i+3];
	}

	for(int i=0;i<4;i++){
		sum_m[0]+=sum[i];
	}
	while(rem>0){
		sum_m[0]+=array[length-rem];
		rem--;
	}

	#elif UNROLL_TYPE == 8
	float sum[8] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	uint32_t rem = length % 8;
	
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length-rem;i+=8){
		sum[0]+= array[i];
		sum[1]+= array[i+1];
		sum[2]+= array[i+2];
		sum[3]+= array[i+3];
		sum[4]+= array[i+4];
		sum[5]+= array[i+5];
		sum[6]+= array[i+6];
		sum[7]+= array[i+7];
	}
	for(int i=0;i<8;i++){
		sum_m[0]+=sum[i];
	}
	while(rem>0){
		sum_m[0]+=array[length-rem];
		rem--;
	}
	#endif

	return sum_m[0];
}


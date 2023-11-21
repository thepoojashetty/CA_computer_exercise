#include "vec_sum.h"

float vec_sum(float *array, uint64_t length) {
	float *ptr=array;
	float sum_m[1] = {0.0f};

	#if UNROLL_TYPE == 1
	float sum = 0.0f;
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length;i++){
		sum = sum + *ptr;
		ptr++;
	}
	sum_m[0] = sum;

	#elif UNROLL_TYPE == 2
	float sum[2] = {0.0f,0.0f};
	uint32_t rem = length % 2;
	
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length-rem;i+=2){
		sum[0]+= *ptr;
		ptr++;
		sum[1]+= *ptr;
		ptr++;
	}
	sum_m[0] = sum[0] + sum[1];
	if(rem){
		sum_m[0]+=*ptr;
	}

	#elif UNROLL_TYPE == 3
	float sum[3] = {0.0f,0.0f,0.0f};
	uint32_t rem = length % 3;
	
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length-rem;i+=3){
		sum[0]+= *ptr;
		ptr++;
		sum[1]+= *ptr;
		ptr++;
		sum[2]+= *ptr;
		ptr++;
	}
	for(int i=0;i<3;i++){
		sum_m[0]+=sum[i];
	}
	while(rem>0){
		sum_m[0]+=*ptr;
		ptr++;
		rem--;
	}

	#elif UNROLL_TYPE == 4
	float sum[4] = {0.0f,0.0f,0.0f,0.0f};
	uint32_t rem = length % 4;
	
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length-rem;i+=4){
		sum[0]+= *ptr;
		ptr++;
		sum[1]+= *ptr;
		ptr++;
		sum[2]+= *ptr;
		ptr++;
		sum[3]+= *ptr;		
		ptr++;
	}
	for(int i=0;i<4;i++){
		sum_m[0]+=sum[i];
	}
	while(rem>0){
		sum_m[0]+=*ptr;
		ptr++;
		rem--;
	}

	#elif UNROLL_TYPE == 8
	float sum[8] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	uint32_t rem = length % 8;
	
	#pragma novector
	#pragma nounroll
	for(int i=0;i<length-rem;i+=8){
		sum[0]+= *ptr;
		ptr++;
		sum[1]+= *ptr;
		ptr++;
		sum[2]+= *ptr;
		ptr++;
		sum[3]+= *ptr;
		ptr++;
		sum[4]+= *ptr;
		ptr++;
		sum[5]+= *ptr;
		ptr++;
		sum[6]+= *ptr;
		ptr++;
		sum[7]+= *ptr;		
		ptr++;			
	}
	for(int i=0;i<8;i++){
		sum_m[0]+=sum[i];
	}
	while(rem>0){
		sum_m[0]+=*ptr;
		ptr++;
		rem--;
	}
	#endif

	return sum_m[0];
}


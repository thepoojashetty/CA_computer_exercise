#include "vec_sum.h"
//#define ONE_FOLD
#define TWO_FOLD
//#define THREE_FOLD
//#define FOUR_FOLD
//#define EIGHT_FOLD*/


float vec_sum(float *array, uint64_t length) {
	float *ptr=array;
	float sum_m[1] = {0.0f};
	#ifdef ONE_FOLD
	float sum = 0.0f;
	#pragma nounroll
	for(int i=0;i<length;i++){
		sum = sum + *ptr;
		ptr++;
	}
	sum_m[0] = sum;
	#endif

	#ifdef TWO_FOLD
	float sum[2] = {0.0f,0.0f};
	uint32_t rem = length % 2;
	#pragma nounroll
	for(int i=0;i<length-rem;i+=2){
		sum[0]+= *ptr;
		ptr++;
		sum[1]+= *ptr;
	}
	sum_m[0] = sum[0] + sum[1];
	#endif

	#ifdef THREE_FOLD
	float sum[3] = {0.0f,0.0f,0.0f};
	uint32_t rem = length % 3;
	#pragma nounroll
	for(int i=0;i<length-rem;i+=3){
		sum[0]+= *ptr;
		ptr++;
		sum[1]+= *ptr;
		ptr++;
		sum[2]+= *ptr;
	}
	for(int i=0;i<3;i++){
		sum_m[0]+=sum[i];
	}		
	#endif

	#ifdef FOUR_FOLD
	float sum[4] = {0.0f,0.0f,0.0f,0.0f};
	uint32_t rem = length % 4;
	#pragma nounroll
	for(int i=0;i<length-rem;i+=4){
		sum[0]+= *ptr;
		ptr++;
		sum[1]+= *ptr;
		ptr++;
		sum[2]+= *ptr;
		ptr++;
		sum[3]+= *ptr;		
	}
	for(int i=0;i<4;i++){
		sum_m[0]+=sum[i];
	}
	#endif

	#ifdef EIGHT_FOLD
	float sum[8] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	uint32_t rem = length % 8;
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
	}
	for(int i=0;i<8;i++){
		sum_m[0]+=sum[i];
	}
	#endif

	return sum_m[0];
}


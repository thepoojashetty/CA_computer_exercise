#include "vec_sum.h"

float vec_sum(float *array, uint64_t length) {
	float sum=0;
	float *ptr=array;
	for(int i=0;i<length;i++){
		sum = sum + *ptr;
		ptr++;
	}
	return sum;
}


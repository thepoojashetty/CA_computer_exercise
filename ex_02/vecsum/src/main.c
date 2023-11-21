#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "vec_sum.h"
#include "get_time.h"

static void usage_msg(void) {
	fprintf(stderr, "Usage: ./vecSum <array size in kiB> <minimal runtime in milliseconds>\n");
	return;
}

int main(int argc, char *argv[]) {
	uint64_t minimal_runtime  = 0u;
	uint64_t actual_runtime   = 0u;
	uint64_t array_size_bytes = 0u;  // The allocated array size in bytes
	uint64_t runs             = 0u;
	uint64_t start            = 0u;
	uint64_t stop             = 0u;
	double  adds_per_second  = 0u;

	if(argc != 3 || argv == NULL) {
		usage_msg();
		return -1;
	}
	
	//TODO: parse parameter: size of the vector in KiB
	//TODO: allocate memory and initialize it
	//TODO: measurement with a runtime of at least 1 s

	array_size_bytes = strtoull(argv[1],NULL,10)*1024;
	minimal_runtime = strtoull(argv[2],NULL,10)*1000;

	uint64_t length= array_size_bytes/sizeof(float);
	float* array= (float*)malloc(length*sizeof(float)); 
	//The size may not be perfect multiple of sizeof(float)

    //the loop keeps doubling the value of runs until actual_runtime exceeds or becomes 
	// equal to minimal_runtime
	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		start = get_time_us();
		for(uint64_t i = 0u; i < runs; i++) {
			// TODO
			vec_sum(array, length);
		}
		stop  = get_time_us();
		actual_runtime = stop - start;
	}
 
	//TODO: calculate and print
	//length number of floating point additions in one vec_sum call
	//method is called for runs number of times
	runs=runs>>1u;
	//fprintf(stdout,"%"PRIu64 "\n",runs);
	//fprintf(stdout,"%"PRIu64 "\n",length);
	//fprintf(stdout,"%"PRIu64 "\n",actual_runtime);

	adds_per_second = ((double)(runs*length))/((double)actual_runtime/1000000.0); // Measured performance as floating point additions per second
	fprintf(stdout, "%" PRIu64 ",%lf,%" PRIu64 ",%" PRIu64 "\n", array_size_bytes, adds_per_second, actual_runtime, minimal_runtime);

	return 0;
}


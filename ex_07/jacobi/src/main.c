#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "jacobi.h"
#include "get_time.h"
#include "draw.h"
#include "math.h"
#include "string.h"
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include "barrier.h"

#ifndef THREADS
#define THREADS (1)
#endif

void create_file_dir(char* path);

static void usage_msg(void) {
	fprintf(stderr, "Usage: ./jacobi <array size in kiB> <minimal runtime in milliseconds> <total number of threads>\n");
	return;
} 

struct work_package_s
{
	double *grid1;
	double *grid2;
	uint32_t size_x;
	uint32_t size_y;
	uint32_t runs;
	uint32_t thread_count;
};

void* worker_thread(void* void_args){
	struct work_package_s *args = (struct work_package_s*) void_args;
	for(uint32_t i = 0u; i < args->runs; i++) {
		// TODO
		jacobi_subgrid(args->grid1,args->grid2,args->size_x,args->size_y);
		sync_barrier(args->thread_count);
		//swap source and target
		double *temp=args->grid1;
		args->grid1=args->grid2;
		args->grid2=temp;
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	uint64_t minimal_runtime  = 0u;
	uint64_t actual_runtime   = 0u;
	uint64_t array_size_bytes = 0u;  // The allocated array size in bytes
	uint64_t runs             = 0u;
	uint64_t start            = 0u;
	uint64_t stop             = 0u;
	double  mup_per_second    = 0u;
	double mup_per_call     = 0u;
	char filepath[100],dirpath[100];
	uint64_t thread_count = THREADS;

	create_file_dir(dirpath);	

	if(argc != 4 || argv == NULL) {
		usage_msg();
		return -1;
	}
	
	//TODO: parse parameter: size of the vector in KiB
	//TODO: allocate memory and initialize it
	//TODO: measurement with a runtime of at least 1 s

	array_size_bytes = strtoull(argv[1],NULL,10)*1048576;
	minimal_runtime = strtoull(argv[2],NULL,10)*1000;
	thread_count = strtoull(argv[3],NULL,10);
	//fprintf(stdout,)

	uint32_t X= sqrt(array_size_bytes/(sizeof(double)*2)); //width
	uint32_t Y=X; //height
	uint32_t size = X*Y;

	double *grid_source = (double*) _mm_malloc(size * sizeof(double),64);
    double *grid_target = (double*) _mm_malloc(size * sizeof(double),64);
	double *temp;

	struct work_package_s pkgs[thread_count];

	//the loop keeps doubling the value of runs until actual_runtime exceeds or becomes 
	// equal to minimal_runtime
	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		for(uint32_t i=0;i<Y;i++){
			for(uint32_t j=0;j<X;j++){
				if (i == 0 || j == 0){
					grid_source[i * X + j] = grid_target[i * X + j] = 1.0;
				}
				else{
					grid_source[i * X + j] = grid_target[i * X + j] = 0.0;
				}
			}
		}
		uint32_t sizey_per_thread = Y/(thread_count);
		uint32_t size_y,address_offset=0;
		for(uint32_t i=0;i<thread_count;i++){
			if(i==thread_count-1){
				size_y = Y - i*sizey_per_thread +1;
				if(i!=0){
					address_offset = (sizey_per_thread*i-1)*X;
				}else{
					size_y = Y;
				}
			}else if(i==0){
				size_y = sizey_per_thread+1;
			}else{
				size_y = sizey_per_thread+2;
				address_offset = (sizey_per_thread*i-1)*X;
			}
			pkgs[i].grid1=grid_source+address_offset;
			pkgs[i].grid2=grid_target+address_offset;
			pkgs[i].size_x=X;
			pkgs[i].size_y=size_y;
			pkgs[i].runs=runs;
			pkgs[i].thread_count=thread_count;
		}

		pthread_t threads[thread_count];

		start = get_time_us();
		for(uint32_t t=0;t<thread_count-1;t++){
			pthread_create(&threads[t],NULL,worker_thread,(void*)&pkgs[t]);
		}
		// worker_thread((void*)&pkgs[thread_count-1]);
		struct work_package_s *args = (struct work_package_s*) &pkgs[thread_count-1];
		for(uint32_t i = 0u; i < args->runs; i++) {
			// TODO
			jacobi_subgrid(args->grid1,args->grid2,args->size_x,args->size_y);
			sync_barrier(args->thread_count);
			//swap source and target
			double *temp=args->grid1;
			args->grid1=args->grid2;
			args->grid2=temp;
		}
		for(uint32_t t=0;t<thread_count-1;t++){
			pthread_join(threads[t],NULL);
		}
		stop  = get_time_us();
		actual_runtime = stop - start;
	}
	
	//TODO: calculate and print
	//length number of floating point additions in one vec_sum call
	//method is called for runs number of times
	if(thread_count==3){
		sprintf(filepath, "%s/grid.ppm",dirpath);
		draw_grid(grid_source,X,Y,filepath);
	}

	runs = runs>>1u;
	mup_per_call = (X-2)*(Y-2)/1.0e6;
	//number of 
	mup_per_second = ((double)(runs*mup_per_call))/((double)actual_runtime/1000000.0); // Measured performance as floating point additions per second
	fprintf(stdout, "%" PRIu64 ",%lf,%" PRIu64 ",%" PRIu64 ",%" PRIu32 ",%" PRIu64 "\n", array_size_bytes, mup_per_second, actual_runtime, minimal_runtime,X,thread_count);

	return 0;
}

void create_file_dir(char* path){
	sprintf(path, "../images/loopunroll_4");
	if (access(path, F_OK) != 0) {
        // Directory does not exist, create it
        if (mkdir(path, 0777) == 0) {
            //Directory created successfully
        }
    } else {
        //Directory already exists
    }
}


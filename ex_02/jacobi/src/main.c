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

void printgrid(double*grid,uint32_t X, uint32_t Y);
void create_file_dir(char* path);

static void usage_msg(void) {
	fprintf(stderr, "Usage: ./jacobi <array size in kiB> <minimal runtime in milliseconds>\n");
	return;
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
	create_file_dir(dirpath);

	if(argc != 3 || argv == NULL) {
		usage_msg();
		return -1;
	}
	
	//TODO: parse parameter: size of the vector in KiB
	//TODO: allocate memory and initialize it
	//TODO: measurement with a runtime of at least 1 s

	array_size_bytes = strtoull(argv[1],NULL,10)*1024;
	minimal_runtime = strtoull(argv[2],NULL,10)*1000;
	//fprintf(stdout,)

	uint32_t X= sqrt(array_size_bytes/(sizeof(double)*2)); //width
	uint32_t Y=X; //height
	uint32_t size = X*Y;

	double *grid_source = (double*) _mm_malloc(size * sizeof(double),64);
    double *grid_target = (double*) _mm_malloc(size * sizeof(double),64);
	double *temp;

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
		start = get_time_us();
		for(uint64_t i = 0u; i < runs; i++) {
			// TODO
			jacobi(grid_source,grid_target,X,Y);
			//swap source and target
			temp=grid_source;
			grid_source=grid_target;
			grid_target=temp;
			//if(i%10==0 && i<=200 && array_size_bytes==4096){
			//	sprintf(filepath, "%s/grid%" PRIu64 ".ppm",dirpath,i);
			//	draw_grid(grid_source,X,Y,filepath);
			//}
		}
		stop  = get_time_us();
		actual_runtime = stop - start;
	}

	if(array_size_bytes==4096){
		sprintf(filepath, "%s/grid.ppm",dirpath);
    	draw_grid(grid_source,X,Y,filepath);
	}
	//TODO: calculate and print
	//length number of floating point additions in one vec_sum call
	//method is called for runs number of times
	runs = runs>>1u;
	mup_per_call = (X-2)*(Y-2)/1.0e6;
	//number of 
	mup_per_second = ((double)(runs*mup_per_call))/((double)actual_runtime/1000000.0); // Measured performance as floating point additions per second
	fprintf(stdout, "%" PRIu64 ",%lf,%" PRIu64 ",%" PRIu64 ",%" PRIu32 "\n", array_size_bytes, mup_per_second, actual_runtime, minimal_runtime,X);

	return 0;
}

void printgrid(double*grid,uint32_t X, uint32_t Y){
	for(uint32_t i=0;i<Y;i++){
			for(uint32_t j=0;j<X;j++){
				fprintf(stdout,"%" PRIu32 ",%" PRIu32 ",%lf ",i,j,grid[i * X + j]);
			}
		fprintf(stdout,"\n");
	}
}

void create_file_dir(char* path){
	sprintf(path, "../images/loopunroll_%d",JACOBIUNROLLTYPE);
	if (access(path, F_OK) != 0) {
        // Directory does not exist, create it
        if (mkdir(path, 0777) == 0) {
            //Directory created successfully
        }
    } else {
        //Directory already exists
    }
}


#include <cuda.h>
#include <cuda_runtime.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <malloc.h>
#include <unistd.h>
#include <immintrin.h>
#include <time.h>
#include <sys/time.h>

uint64_t get_time_us(void) {
    struct timespec a;
    clock_gettime(CLOCK_MONOTONIC, &a);
    return (uint64_t) (((double) a.tv_nsec / 1000.0) + ((double) a.tv_sec * (1000.0 * 1000.0)));
}

// CUDA kernel for jacobi iteration on the GPU
__global__ void jacobi(double* grid_source, double* grid_target, uint32_t *_X, uint32_t *_Y) {
    uint32_t X = *_X;
    uint32_t Y = *_Y;
    uint32_t x = blockIdx.x * blockDim.x + threadIdx.x;
    uint32_t y = blockIdx.y * blockDim.y + threadIdx.y;
    if(x>0 && x<X-1 && y>0 && y<Y-1) {
        grid_target[y*X+x] = 0.25 * (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]);
    }
}

int main() {
    uint64_t start, end;
    uint32_t sizeGib = 3;
    uint64_t sizeBytes = sizeGib*1024*1024*1024;
    uint32_t X = sqrt(sizeBytes/sizeof(double));
    uint32_t Y = X;
    uint32_t size = X*Y;
    uint64_t minimum_runtime_ms = 100;
    uint64_t minimal_runtime_us = minimum_runtime_ms * 1000u;

    printf("Minimum runtime: %" PRIu64 " ms\n", minimum_runtime_ms);

    // Allocate memory on the host
    double *grid_source = (double*) _mm_malloc(size*sizeof(double),64);
    double *grid_target = (double*) _mm_malloc(size*sizeof(double),64);

    double *d_grid_source, *d_grid_target;
    cudaMalloc((void**)&d_grid_source, size*sizeof(double));
    cudaMalloc((void**)&d_grid_target, size*sizeof(double));

    uint32_t *d_X, *d_Y;
    cudaMalloc((void**)&d_X, sizeof(uint32_t));
    cudaMalloc((void**)&d_Y, sizeof(uint32_t));

    int threadsPerBlockX = 32;
    int threadsPerBlockY = 32;

    dim3 threadsPerBlock(threadsPerBlockX, threadsPerBlockY);
    dim3 blocksPerGrid((X + threadsPerBlockX - 1) / threadsPerBlockX, (Y + threadsPerBlockY - 1) / threadsPerBlockY);


    start = get_time_us();
    // cudaMemcpy(d_A, A, GB, cudaMemcpyHostToDevice);
    cudaMemcpy(d_grid_source, grid_source, size*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_grid_target, grid_target, size*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_X, &X, sizeof(uint32_t), cudaMemcpyHostToDevice);
    cudaMemcpy(d_Y, &Y, sizeof(uint32_t), cudaMemcpyHostToDevice);
    end = get_time_us();
    double toGPU_s = ((double)(end - start))/1000000.0;
    printf("Copying data to gpu took: %lf seconds\n", toGPU_s);


    // Launch the jacobi kernel
    uint64_t actual_runtime = 0u;
    uint64_t runs           = 0u;
    double *temp;
    for(runs = 1u; actual_runtime < minimal_runtime_us; runs = runs << 1u) {
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
			jacobi<<<blocksPerGrid, threadsPerBlock>>>(d_grid_source, d_grid_target, d_X, d_Y);
            cudaDeviceSynchronize();
			//swap source and target
			temp=d_grid_source;
			d_grid_source=d_grid_target;
			d_grid_target=temp;
		}
		end = get_time_us();
		actual_runtime = end - start;
	}
    runs = runs>>1u;
    uint32_t n=(X-2)*(Y-2);
    double runKernel_s = ((double)actual_runtime)/1000000.0;
    printf("Running kernel took: %lf seconds\n", runKernel_s);

    // Copy the results back to the host
    start = get_time_us();
    cudaMemcpy(grid_target, d_grid_source, size*sizeof(double), cudaMemcpyDeviceToHost);
    end = get_time_us();
    double toCPU_s = ((double)(end - start))/1000000.0;
    printf("Copying data to cpu took: %lf seconds\n", toCPU_s);

    // Compute the bandwidth
    printf("Internal memory GPU bandwidth : %lf GB/s\n", (double)(runs*6)/runKernel_s);
    printf("External memory GPU bandwidth : %lf GB/s\n", (double)(runs*6)/(toGPU_s+runKernel_s+toCPU_s));

    // Free allocated memory on the GPU
    cudaFree(d_grid_source);
    cudaFree(d_grid_target);

    // Free allocated memory on the CPU
    free(grid_source);
    free(grid_target);
    return 0;
}

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

const int n = 134217728; // no of double elements in the array


uint64_t get_time_us(void) {
    struct timespec a;
    clock_gettime(CLOCK_MONOTONIC, &a);
    return (uint64_t) (((double) a.tv_nsec / 1000.0) + ((double) a.tv_sec * (1000.0 * 1000.0)));
}

__global__ void update_grid(double *src, const double *tgt, int width, int height) {
    int tidx = blockIdx.x * blockDim.x + threadIdx.x;
    int tidy = blockIdx.y * blockDim.y + threadIdx.y;
    if ((tidx < width) && (tidy < height)) {
        ;
    }
}
__device__ unsigned long long get_time_us_device() {
    return clock64();
}

// Host function to call the device function
unsigned long long get_time_us_host() {
    unsigned long long result;
    cudaMemcpyFromSymbol(&result, get_time_us_device, sizeof(unsigned long long));
    return result;
}

int main() {
    uint64_t start, end,size;
    // Allocate and initialize arrays B and C on the CPU
    printf("size of n");
    printf("%d\n", n);

    size=n*sizeof(double);
    // Allocate memory on the host
    double *gridSrcHst = (double*) _mm_malloc(size,64);
    double *gridTgtHst = (double*) _mm_malloc(size,64);
    int X,Y;
    X = int(sqrt(n));
    Y=X;
    for(uint32_t i=0;i<Y;i++){
        for(uint32_t j=0;j<X;j++){
            if (i == 0 || j == 0){
                gridSrcHst[i * X + j] = gridTgtHst[i * X + j] = 1.0;
            }
            else{
                gridSrcHst[i * X + j] = gridTgtHst[i * X + j] = 0.0;
            }
        }
    }

    double *gridSrcDev, *gridTgtDev; 
    cudaMalloc((void**)&gridSrcDev, size);
    cudaMalloc((void**)&gridTgtDev, size);

    start = get_time_us();
    cudaMemcpy(gridSrcDev, gridSrcHst, size, cudaMemcpyHostToDevice);
    cudaMemcpy(gridTgtDev, gridTgtHst, size, cudaMemcpyHostToDevice);

    end = get_time_us();
    double toGPU_s = ((double)(end - start))/1000000.0;

    printf("Copying data to gpu took: %lf seconds\n", toGPU_s);

    int threadsPerBlock = 512;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    // Launch the STREAM Triad kernel
    start = get_time_us();
    //streamTriad<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, c, n);
    cudaDeviceSynchronize();
    end = get_time_us();
    double runKernel_s = ((double)(end - start))/1000000.0;
    printf("Running kernel took: %lf seconds\n", runKernel_s);

    // Copy the results back to the host
    start = get_time_us();
    cudaMemcpy(gridTgtHst, gridTgtDev, size, cudaMemcpyDeviceToHost);
    end = get_time_us();
    double toCPU_s = ((double)(end - start))/1000000.0;
    printf("Copying data to cpu took: %lf seconds\n", toCPU_s);

    // Print the results
    printf("gridTgtHst[0] = %f\n", gridTgtHst[0]);
    printf("gridTgtHst[n-1] = %f\n", gridTgtHst[n-1]);

    // Compute the bandwidth
    printf("Internal memory GPU bandwidth : %lf GB/s\n", (double)3/runKernel_s);
    printf("External memory GPU bandwidth : %lf GB/s\n", (double)3/(toGPU_s+runKernel_s+toCPU_s));

    // Free allocated memory on the GPU
    cudaFree(gridTgtDev);
    cudaFree(gridSrcDev);

    // Free allocated memory on the CPU
    free(gridTgtHst);
    free(gridSrcHst);
    return 0;
}

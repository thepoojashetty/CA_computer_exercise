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
const double c = 2.0f; // Arbitrary constant (unequal to {0.0, 1.0})

uint64_t get_time_us(void) {
    struct timespec a;
    clock_gettime(CLOCK_MONOTONIC, &a);
    return (uint64_t) (((double) a.tv_nsec / 1000.0) + ((double) a.tv_sec * (1000.0 * 1000.0)));
}

// CUDA kernel for STREAM Triad
__global__ void streamTriad(double *A, const double *B, const double *C, const double c, int N) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < N) {
        A[tid] = B[tid] * c + C[tid];
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
    double *h_A = (double*) _mm_malloc(size,64);
    double *h_B = (double*) _mm_malloc(size,64);
    double *h_C = (double*) _mm_malloc(size,64);
    for (int i = 0; i < n; ++i) {
        h_B[i] = 1;
        h_C[i] = 1;
    }

    double *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    start = get_time_us();
    // cudaMemcpy(d_A, A, GB, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_C, h_C, size, cudaMemcpyHostToDevice);
    end = get_time_us();
    double toGPU_s = ((double)(end - start))/1000000.0;

    printf("Copying data to gpu took: %lf seconds\n", toGPU_s);

    int threadsPerBlock = 512;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    // Launch the STREAM Triad kernel
    start = get_time_us();
    streamTriad<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, c, n);
    cudaDeviceSynchronize();
    end = get_time_us();
    double runKernel_s = ((double)(end - start))/1000000.0;
    printf("Running kernel took: %lf seconds\n", runKernel_s);

    // Copy the results back to the host
    start = get_time_us();
    cudaMemcpy(h_A, d_A, size, cudaMemcpyDeviceToHost);
    end = get_time_us();
    double toCPU_s = ((double)(end - start))/1000000.0;
    printf("Copying data to cpu took: %lf seconds\n", toCPU_s);

    // Print the results
    printf("A[0] = %f\n", h_A[0]);
    printf("A[n-1] = %f\n", h_A[n-1]);

    // Compute the bandwidth
    printf("Internal memory GPU bandwidth : %lf GB/s\n", (double)3/runKernel_s);
    printf("External memory GPU bandwidth : %lf GB/s\n", (double)3/(toGPU_s+runKernel_s+toCPU_s));

    // Free allocated memory on the GPU
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Free allocated memory on the CPU
    free(h_B);
    free(h_C);
    free(h_A);
    return 0;
}

#include <cuda.h>
#include <curand.h>
#include <cublas_v2.h>
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
// Fill the array A(nr_rows_A, nr_cols_A) with random numbers on GPU
void GPU_fill_rand(double *A, int nr_rows_A, int nr_cols_A) {
    // Create a pseudo-random number generator
    curandGenerator_t prng;
    curandCreateGenerator(&prng, CURAND_RNG_PSEUDO_DEFAULT);

    // Set the seed for the random number generator using the system clock
    curandSetPseudoRandomGeneratorSeed(prng, (unsigned long long) clock());

    // Fill the array with random numbers on the device
    curandGenerateUniformDouble(prng, A, nr_rows_A * nr_cols_A);
}

// CUDA kernel for jacobi iteration on the GPU
__global__ void matrixMul(const double *A, const double *B, double *C, const int m, const int k, const int n) {
    int lda=m,ldb=k,ldc=m;
    const double alf = 1;
    const double bet = 0;
    const double *alpha = &alf;
    const double *beta = &bet;

    // Create a handle for CUBLAS
    cublasHandle_t handle;
    cublasCreate(&handle);

    // Do the actual multiplication
    cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Destroy the handle
    cublasDestroy(handle);
}

int main() {

    uint64_t start, end;
    uint32_t sizeGiB = 2.6;
    uint64_t sizeBytes = sizeGiB*1024*1024*1024;
    uint32_t X = sqrt(sizeBytes/sizeof(double));
    uint32_t Y = X;
    uint32_t size = X*Y;
    uint64_t minimum_runtime_ms = 100;

    double *h_A = (double *)_mm_malloc(size*sizeof(double),64);
    double *h_B = (double *)_mm_malloc(size*sizeof(double),64);
    double *h_C = (double *)_mm_malloc(size*sizeof(double),64);
    printf("Minimum runtime: %" PRIu64 " ms\n", minimum_runtime_ms);

    double *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size*sizeof(double));
    cudaMalloc((void**)&d_B, size*sizeof(double));
    cudaMalloc((void**)&d_C, size*sizeof(double));
    GPU_fill_rand(d_A, X, Y);
    GPU_fill_rand(d_B, X, Y);

    start = get_time_us();
    // cudaMemcpy(d_A, A, GB, cudaMemcpyHostToDevice);
    cudaMemcpy(d_A, h_A, size*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size*sizeof(double), cudaMemcpyHostToDevice);
    end = get_time_us();
    double toGPU_s = ((double)(end - start))/1000000.0;
    printf("Copying data to gpu took: %lf seconds\n", toGPU_s);

    int threadsPerBlockX = 32;
    int threadsPerBlockY = 32;

    dim3 threadsPerBlock(threadsPerBlockX, threadsPerBlockY);
    dim3 blocksPerGrid((X + threadsPerBlockX - 1) / threadsPerBlockX, (Y + threadsPerBlockY - 1) / threadsPerBlockY);
    uint64_t actual_runtime = 0u;
    start = get_time_us();
    // Launch the matrix multiplication kernel
    matrixMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, X, Y, Y);
    cudaDeviceSynchronize();
    end = get_time_us();
    actual_runtime = end - start;

    /*uint64_t actual_runtime = 0u;
    uint64_t runs           = 0u;
    for(runs = 1u; actual_runtime < minimal_runtime_us; runs = runs << 1u) {
		start = get_time_us();
		for(uint64_t i = 0u; i < runs; i++) {
			matrixMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, X, Y, Y);
            cudaDeviceSynchronize();
		}
		end = get_time_us();
		actual_runtime = end - start;
	}
    runs = runs>>1u;
    uint32_t n=(X-2)*(Y-2);*/
    double runKernel_s = ((double)actual_runtime)/1000000.0;
    printf("Running kernel took: %lf seconds\n", runKernel_s);

    // Copy the results back to the host
    start = get_time_us();
    cudaMemcpy(h_C, d_C, size*sizeof(double), cudaMemcpyDeviceToHost);
    end = get_time_us();
    double toCPU_s = ((double)(end - start))/1000000.0;
    printf("Copying data to cpu took: %lf seconds\n", toCPU_s);

    // Compute the bandwidth
    printf("Internal memory GPU bandwidth : %lf GB/s\n", (double)3/runKernel_s);
    printf("External memory GPU bandwidth : %lf GB/s\n", (double)3/(toGPU_s+runKernel_s+toCPU_s));

    // Free allocated memory on the GPU
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Free allocated memory on the CPU
    free(h_A);
    free(h_B);
    free(h_C);
    return 0;
}

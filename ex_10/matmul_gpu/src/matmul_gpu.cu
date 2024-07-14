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

// CUDA kernel for cublas matrix multiplication
void matrixMul(const double *A, const double *B, double *C, const int m, const int k, const int n) {
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
    uint64_t sizeBytes = 2.6*1024*1024*1024;
    uint32_t X = sqrt(sizeBytes/sizeof(double));
    uint32_t Y = X;
    uint32_t size = X*Y;
    uint64_t runtime = 0u;
    uint64_t start, end;

    double *h_A = (double *)_mm_malloc(size*sizeof(double),64);
    double *h_B = (double *)_mm_malloc(size*sizeof(double),64);
    double *h_C = (double *)_mm_malloc(size*sizeof(double),64);

    double *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, size*sizeof(double));
    cudaMalloc((void**)&d_B, size*sizeof(double));
    cudaMalloc((void**)&d_C, size*sizeof(double));

    GPU_fill_rand(d_A, X, Y);
    GPU_fill_rand(d_B, X, Y);

    start = get_time_us();
    // Launch the matrix multiplication
    matrixMul(d_A, d_B, d_C, X, Y, Y);
    end = get_time_us();
    runtime = end - start;

    double runtime_s = ((double)runtime)/1000000.0;
    printf("Running matmul took: %lf seconds\n", runtime_s);

    // Copy the results back to the host
    // start = get_time_us();
    cudaMemcpy(h_C, d_C, size*sizeof(double), cudaMemcpyDeviceToHost);
    // end = get_time_us();
    // double toCPU_s = ((double)(end - start))/1000000.0;
    // printf("Copying data to cpu took: %lf seconds\n", toCPU_s);

    // Compute the bandwidth
    printf("Floating point performance with GPU : %lf Flops/s\n", (double)(2*pow(X,3))/runtime_s);

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

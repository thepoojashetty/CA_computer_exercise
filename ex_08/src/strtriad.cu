#include <cuda_runtime.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <malloc.h>
#include <unistd.h>
#include <immintrin.h>


const int n = 2147483648; // 2 GiB per array
const double c = 2.0f; // Arbitrary constant (unequal to {0.0, 1.0})

// CUDA kernel for STREAM Triad
__global__ void streamTriad(double *A, const double *B, const double *C, const double c, int size) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < size) {
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
    // Allocate and initialize arrays B and C on the CPU
    /*unsigned long long start = 0u;
	unsigned long long stop  = 0u;
    unsigned long long runtime = 0u;*/
    /*float *h_B = new float[n];
    float *h_C = new float[n];*/
    printf("size of n");
    printf("%" PRId64 "\n", n);

    // Allocate memory on the host
    double *h_B = (double*) _mm_malloc(n*sizeof(double),64);
    double *h_C = (double*) _mm_malloc(n*sizeof(double),64);
    for (int i = 0; i < n; ++i) {
        h_B[i] = static_cast<double>(i);
        h_C[i] = static_cast<double>(n - i);
    }
    double *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, n*sizeof(double));
    cudaMalloc((void**)&d_B, n*sizeof(double));
    cudaMalloc((void**)&d_C, n*sizeof(double));
    // Create CUDA events for timing
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float milliseconds = 0;
    float dataTxTime = 0;
    float execTime = 0;
    float totalExecTime = 0;
    // Copy arrays B and C from host to device
    cudaEventRecord(start);
    cudaMemcpy(d_B, h_B, n*sizeof(double) , cudaMemcpyHostToDevice);
    cudaMemcpy(d_C, h_C, n*sizeof(double) , cudaMemcpyHostToDevice);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    dataTxTime+=milliseconds;

    printf("Host to device data transfer time: %f ms\n",milliseconds);

    // Configure kernel launch parameters
    /*int blockSize = 512; //512 threads per block
    int gridSize = (n + blockSize - 1) / blockSize;*/
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((n*sizeof(double) + threadsPerBlock.x -1) / threadsPerBlock.x, (n*sizeof(double)+threadsPerBlock.y -1) / threadsPerBlock.y);
    // Launch the STREAM Triad kernel

    // Record start time
    cudaEventRecord(start);
    printf("Running kernel now!\n");
    streamTriad<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, c, n);
    printf("done executing kernel !\n");
    // Record stop time
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    execTime+=milliseconds;
    totalExecTime = execTime+dataTxTime;
    printf("Kernel execution time: %f ms\n",execTime);
    printf("Total execution time: %f ms\n",totalExecTime);
    // Copy array A from device to host for verification
    cudaMemcpy(h_B, d_A, n, cudaMemcpyDeviceToHost);

    // Free allocated memory on the GPU
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Destroy CUDA events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    // Free allocated memory on the CPU
    delete[] h_B;
    delete[] h_C;

    return 0;
}

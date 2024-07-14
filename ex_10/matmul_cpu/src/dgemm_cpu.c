/* C source code is found in dgemm_example.c */
#define _POSIX_C_SOURCE 199309L
#define min(x,y) (((x) < (y)) ? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <malloc.h>
#include <unistd.h>
#include <immintrin.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "mkl.h"

uint64_t get_time_us(void) {
    struct timespec a;
    clock_gettime(CLOCK_MONOTONIC, &a);
    return (uint64_t) (((double) a.tv_nsec / 1000.0) + ((double) a.tv_sec * (1000.0 * 1000.0)));
}

int main()
{
    uint64_t sizeBytes = 15ULL*1024*1024*1024;
    uint64_t X = sqrt(sizeBytes/sizeof(double));
    uint64_t Y = X;
    uint64_t size = X*Y;
    uint64_t runtime = 0u;
    uint64_t start, end;

    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;

    //C=alpha*A*B+beta*C 
    m = k = n = X;
    alpha = 1.0; beta = 0.0;

    A = (double *)mkl_malloc( m*k*sizeof( double ), 64 );
    B = (double *)mkl_malloc( k*n*sizeof( double ), 64 );
    C = (double *)mkl_malloc( m*n*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
      printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
      mkl_free(A);
      mkl_free(B);
      mkl_free(C);
      return 1;
    }

    printf (" Intializing matrix data \n\n");
    for (i = 0; i < (m*k); i++) {
        A[i] = (double)(i+1);
    }

    for (i = 0; i < (k*n); i++) {
        B[i] = (double)(-i-1);
    }

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    printf (" Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface \n\n");
    start = get_time_us();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);
    end = get_time_us();
    printf ("\n Computations completed.\n\n");
    runtime = end - start;
    double runtime_s = ((double)runtime)/1000000.0;
    printf("Running matmul took: %lf seconds\n", runtime_s);

    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(k,6); j++) {
        printf ("%12.0f", A[j+i*k]);
      }
      printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(k,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.0f", B[j+i*n]);
      }
      printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.5G", C[j+i*n]);
      }
      printf ("\n");
    }

    printf("Floating point performance with CPU : %lf Flops/s\n", (double)(2*pow(X,3))/runtime_s);

    printf ("\n Deallocating memory \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    printf (" Example completed. \n\n");
    return 0;
}
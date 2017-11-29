
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include <mkl.h>
#include <mkl_types.h>
#include <mkl_cblas.h>
#include <mkl_blas.h>
#include <mkl_lapack.h>

#ifndef NUM_CACHE_LINES
/* L2 cache: 256 K */
#define NUM_CACHE_LINES ((256 * 1024) / 64)
#endif

#define _min_(a,b) (((a) < (b)) ? a : b)

#define SIZE_K SIZE
#define SIZE_N SIZE
#define SIZE_M SIZE

void mmult(
  const double A[SIZE][SIZE],
  const double B[SIZE][SIZE],
  double C[SIZE][SIZE]
) {
/*
  void cblas_dgemm(const CBLAS_LAYOUT    Layout,
                   const CBLAS_TRANSPOSE TransA,
                   const CBLAS_TRANSPOSE TransB,
                   const MKL_INT         M,
                   const MKL_INT         N,
                   const MKL_INT         K,
                   const double          alpha,
                   const double *        A,
                   const MKL_INT         lda,
                   const double *        B,
                   const MKL_INT         ldb,
                   const double          beta,
                   double *              C,
                   const MKL_INT         ldc);

*/
  cblas_dgemm(CblasRowMajor,
              CblasNoTrans,
              CblasNoTrans,
              SIZE_M,
              SIZE_N,
              SIZE_K,
              1.0, 
              (const double *)A,
              SIZE_K,
              (const double *)B,
              SIZE_N,
              0.0,
              (double*)C,
              SIZE_N);
}


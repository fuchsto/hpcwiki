
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "immintrin.h"
#include "smmintrin.h"
#include "emmintrin.h"

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
  int i0, j0, k0, i, j, k;
  /* Set to 280 in solution HB, should be squrt(NUM_CACHE_LINES) */
  int B_SIZE = 64;

  __m256d Ymm_A;
  __m256d sum1;
  __m256d sum2;
  __m256d sum3;
  __m256d sum4;

  for (i0 = 0; i0 < SIZE_M; i0 += B_SIZE) {
    for (j0 = 0; j0 < SIZE_K; j0 += B_SIZE) {
      for (k0 = 0; k0 < SIZE_N; k0 += B_SIZE) {
        for (i = i0; i < _min_(i0+B_SIZE, SIZE_M); i += 4) {
          for (j = j0; j < _min_(j0+B_SIZE, SIZE_K); j += 4) {
            sum1 = _mm256_set1_pd(0.0);
            sum2 = _mm256_set1_pd(0.0);
            sum3 = _mm256_set1_pd(0.0);
            sum4 = _mm256_set1_pd(0.0);
            
            for (k = k0; k < _min_(k0+B_SIZE, SIZE_N); k += 4) {
              // ein register je reihe fuer B
              __m256d mb1, mb2, mb3, mb4;

              // ein register je element/reihe fuer A
              __m256d ma1, ma2, ma3, ma4;

              // register fuer A fuellen
              ma1 = _mm256_broadcast_sd(&A[i][k]);
              ma2 = _mm256_broadcast_sd(&A[i][k+1]);
              ma3 = _mm256_broadcast_sd(&A[i][k+2]);
              ma4 = _mm256_broadcast_sd(&A[i][k+3]);

              // register mit reihe aus fuellen

              // mit cheat
              mb1 = _mm256_load_pd(&B[k][j]);
              mb2 = _mm256_load_pd(&B[k+1][j]);
              mb3 = _mm256_load_pd(&B[k+2][j]);
              mb4 = _mm256_load_pd(&B[k+3][j]);

              //mult
              ma1 = _mm256_mul_pd(ma1, mb1);
              ma2 = _mm256_mul_pd(ma2, mb2);
              ma1 = _mm256_add_pd(ma1, ma2);

              ma3 = _mm256_mul_pd(ma3, mb3);
              ma4 = _mm256_mul_pd(ma4, mb4);
              ma3 = _mm256_add_pd(ma3, ma4);

              ma1 = _mm256_add_pd(ma1, ma3);
              sum1 = _mm256_add_pd(ma1, sum1);

              //reihe 2
              ma1 = _mm256_broadcast_sd(&A[i+1][k]);
              ma2 = _mm256_broadcast_sd(&A[i+1][k+1]);
              ma3 = _mm256_broadcast_sd(&A[i+1][k+2]);
              ma4 = _mm256_broadcast_sd(&A[i+1][k+3]);

              ma1 = _mm256_mul_pd(ma1, mb1);
              ma2 = _mm256_mul_pd(ma2, mb2);
              ma1 = _mm256_add_pd(ma1, ma2);

              ma3 = _mm256_mul_pd(ma3, mb3);
              ma4 = _mm256_mul_pd(ma4, mb4);
              ma3 = _mm256_add_pd(ma3, ma4);

              ma1 = _mm256_add_pd(ma1, ma3);
              sum2 = _mm256_add_pd(ma1, sum2);

              //reihe 3
              ma1 = _mm256_broadcast_sd(&A[i+2][k]);
              ma2 = _mm256_broadcast_sd(&A[i+2][k+1]);
              ma3 = _mm256_broadcast_sd(&A[i+2][k+2]);
              ma4 = _mm256_broadcast_sd(&A[i+2][k+3]);

              ma1 = _mm256_mul_pd(ma1, mb1);
              ma2 = _mm256_mul_pd(ma2, mb2);
              ma1 = _mm256_add_pd(ma1, ma2);

              ma3 = _mm256_mul_pd(ma3, mb3);
              ma4 = _mm256_mul_pd(ma4, mb4);
              ma3 = _mm256_add_pd(ma3, ma4);

              ma1 = _mm256_add_pd(ma1, ma3);
              sum3 = _mm256_add_pd(ma1, sum3);

              //reihe 4
              ma1 = _mm256_broadcast_sd(&A[i+3][k]);
              ma2 = _mm256_broadcast_sd(&A[i+3][k+1]);
              ma3 = _mm256_broadcast_sd(&A[i+3][k+2]);
              ma4 = _mm256_broadcast_sd(&A[i+3][k+3]);

              ma1 = _mm256_mul_pd(ma1, mb1);
              ma2 = _mm256_mul_pd(ma2, mb2);
              ma1 = _mm256_add_pd(ma1, ma2);

              ma3 = _mm256_mul_pd(ma3, mb3);
              ma4 = _mm256_mul_pd(ma4, mb4);
              ma3 = _mm256_add_pd(ma3, ma4);

              ma1 = _mm256_add_pd(ma1, ma3);
              sum4 = _mm256_add_pd(ma1, sum4);
            }

            Ymm_A = _mm256_load_pd(&C[i][j]);
            Ymm_A = _mm256_add_pd(Ymm_A, sum1);
            _mm256_store_pd(&C[i][j], Ymm_A);

            Ymm_A = _mm256_load_pd(&C[i+1][j]);
            Ymm_A = _mm256_add_pd(Ymm_A, sum2);
            _mm256_store_pd(&C[i+1][j], Ymm_A);

            Ymm_A = _mm256_load_pd(&C[i+2][j]);
            Ymm_A = _mm256_add_pd(Ymm_A, sum3);
            _mm256_store_pd(&C[i+2][j], Ymm_A);

            Ymm_A = _mm256_load_pd(&C[i+3][j]);
            Ymm_A = _mm256_add_pd(Ymm_A, sum4);
            _mm256_store_pd(&C[i+3][j], Ymm_A);
          }
        }
      }
    }
  }
}



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
  int    i,  j,  k,
         i0, j0, k0,
         TS;
  double a_tmp, b_tmp;

  TS = (int)(sqrt(NUM_CACHE_LINES));

  assert((SIZE / TS) * TS == SIZE);
  assert(sizeof(double) == 8);

/*
  Notes:
  __m256d : 256-bit vector containing 4 doubles
  _pd     : vectors contain doubles (Packed Double-precision)
*/
  __m256d sum;
  __m256d c_i_j;

  for (k0 = 0; k0 < SIZE; k0 += TS) {
    for (j0 = 0; j0 < SIZE; j0 += TS) {
      for (i = 0; i < SIZE; i++) {
        for (k = k0; k < _min_(k0 + TS, SIZE); k += 4) {
          /* a_tmp = A[i][k..k+3]; */
          __m256d a_kv = _mm256_load_pd(&A[i][k]);
          sum = _mm256_set1_pd(0.0);
          for (j = j0; j < _min_(j0 + TS, SIZE); j += 4) {
            /* b_k$_jv = B[k$][j..j+3] */
            __m256d b_k0_jv = _mm256_load_pd(&B[k][j]);
            __m256d b_k1_jv = _mm256_load_pd(&B[k+1][j]);
            __m256d b_k2_jv = _mm256_load_pd(&B[k+2][j]);
            __m256d b_k3_jv = _mm256_load_pd(&B[k+3][j]);

            /* b_k$_jv = a_kv * b_k$_jv */
            b_k0_jv = _mm256_mul_pd(b_k0_jv, a_kv);
            b_k1_jv = _mm256_mul_pd(b_k1_jv, a_kv);
            b_k2_jv = _mm256_mul_pd(b_k2_jv, a_kv);
            b_k3_jv = _mm256_mul_pd(b_k3_jv, a_kv);

            sum     = _mm256_add_pd(b_k0_jv, sum);
            sum     = _mm256_add_pd(b_k1_jv, sum);
            sum     = _mm256_add_pd(b_k2_jv, sum);
            sum     = _mm256_add_pd(b_k3_jv, sum);

            /* C[i][j..j+3] += sum[j..j+3] */
            c_i_j   = _mm256_load_pd(&C[i][j]);
            c_i_j   = _mm256_add_pd(c_i_j, sum);

            _mm256_store_pd(&C[i][j], c_i_j);
          }
        }
      }
    }
  }
}


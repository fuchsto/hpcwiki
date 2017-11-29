
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
         i2, j2, k2,
         TS;
  double * rres;
  const double * rmul1;
  const double * rmul2;

  TS = 8;

  assert((SIZE / TS) * TS == SIZE);
  assert(sizeof(double) == 8);

  for (i = 0; i < SIZE_M; i+= TS) {
    for (j = 0; j < SIZE_K; j+=TS) {
      for (k = 0; k < SIZE_N; k+=TS) {
        for(i2 = 0, rres = &C[i][j], rmul1 = &A[i][k]; 
          i2<TS; ++i2, rres+= SIZE_N, rmul1 += SIZE_M){
          _mm_prefetch((char*)(&rmul1[16]), _MM_HINT_NTA);
          for(k2=0, rmul2 = &B[k][j];k2<TS; ++k2,rmul2+=SIZE_K){
            __m256d m1 = _mm256_broadcast_sd(&rmul1[k2]);
            __m256d m2 = _mm256_load_pd(&rmul2[0]);
            __m256d r2 = _mm256_load_pd(&rres[0]);
            _mm256_store_pd(&rres[0], _mm256_add_pd(_mm256_mul_pd(m2,m1), r2));

            m2 = _mm256_load_pd(&rmul2[4]);
            r2 = _mm256_load_pd(&rres[4]);
            _mm256_store_pd(&rres[4], _mm256_add_pd(_mm256_mul_pd(m2,m1), r2));
          }
        }
      }
    }
  }
}


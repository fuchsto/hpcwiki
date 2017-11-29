
#include <math.h>
#include <assert.h>
#include <stdio.h>

#ifndef NUM_CACHE_LINES
/* L2 cache: 256 K */
#define NUM_CACHE_LINES ((256 * 1024) / 64)
#endif

#define _min_(a,b) (((a) < (b)) ? a : b)

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

  for (k0 = 0; k0 < SIZE; k0 += TS) {
    for (j0 = 0; j0 < SIZE; j0 += TS) {
      for (i = 0; i < SIZE; i++) {
        for (k = k0; k < _min_(k0 + TS, SIZE); k++) {
          a_tmp = A[i][k];
          for (j = j0; j < _min_(j0 + TS, SIZE); j++) {
            b_tmp = B[k][j];
            C[i][j] += a_tmp * b_tmp;
          }
        }
      }
    }
  }
}


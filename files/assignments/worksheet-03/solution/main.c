#define __GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>

#ifdef USE_PAPI
#include <papi.h>
#endif

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

#ifdef ALIGNED
#define ALIGNED_VARIANT ".aligned"
#else
#define ALIGNED_VARIANT ""
#endif

#ifndef SIZE
#define SIZE 100
#endif

#ifndef SIZE_M
#define SIZE_M SIZE
#endif

#ifndef SIZE_N
#define SIZE_N SIZE
#endif

#ifndef SIZE_K
#define SIZE_K SIZE
#endif

#define long_long_t long long

#ifdef ALIGNED
double A[SIZE_M][SIZE_N] __attribute__((aligned(32)));
double B[SIZE_N][SIZE_K] __attribute__((aligned(32)));
double C[SIZE_M][SIZE_K] __attribute__((aligned(32)));
#else
double A[SIZE_M][SIZE_N];
double B[SIZE_N][SIZE_K];
double C[SIZE_M][SIZE_K];
#endif

static inline long_long_t timestamp ();


extern void mmult(
  double A[SIZE][SIZE],
  double B[SIZE][SIZE],
  double C[SIZE][SIZE]
);

int main(int argc, char *argv[])
{
  int         i, j, k;
  double      sum;
  double      nflop;
  long_long_t tstart, tstop;
  double      tmmult;
#ifdef USE_PAPI
  int         ret;
  int         events[2] = { PAPI_L1_DCM, PAPI_FP_OPS };
  long_long_t perfcnt[2];

  if (PAPI_num_counters() < 2) {
    fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
    return EXIT_FAILURE;
  }
#endif

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      A[i][j] = (double)(i+1) + (double)(j+1);
      B[i][j] = (double)(i+1) + (double)(j+1);
      C[i][j] = 0.0;
    }
  }

  /* Two FLOP in inner loop: add and mul */
  nflop = 2.0 * (double)SIZE * (double)SIZE * (double)SIZE;

#ifdef USE_PAPI
  /* Reset performance counters: */
  if ((ret = PAPI_start_counters(events, 2)) != PAPI_OK) {
    fprintf(stderr, "PAPI failed to start counters: %s\n",
                    PAPI_strerror(ret));
    return EXIT_FAILURE;
  }
#endif

  tstart = timestamp();
  mmult(A, B, C);
  tstop = timestamp();

#ifdef USE_PAPI
  /* Read performance counters: */
  if ((ret = PAPI_read_counters(perfcnt, 2)) != PAPI_OK) {
    fprintf(stderr, "PAPI failed to read counters: %s\n",
                    PAPI_strerror(ret));
    return EXIT_FAILURE;
  }
#endif

  /* Duration in nanoseconds.
   * FLOP/ns = GFLOP/s
   */
  tmmult = (double)(tstop - tstart);

  /* Sum of matrix diagonal as correctness hint: */
  sum = 0.0;
  for (i = 0, j = 0; i < SIZE; i++, j++) {
    sum += C[i][j];
  }

#ifdef USE_PAPI
  fprintf(stderr,
          "                variant,      n,   time_ms,       gflops,    hw_flops,   l2_misses\n");
  fprintf(stdout,
          "%*s, %*d, %*.3f, %*f, %*.3f, %*lld \n",
          23,
	  STRINGIZE_VALUE_OF(VARIANT) ALIGNED_VARIANT,
          6, SIZE,
          9, 1.0e-6 * tmmult,
          12, nflop / tmmult,
          12, (float)perfcnt[1],
          12, perfcnt[0]);
#else
  fprintf(stderr,
          "                variant,      n,   time_ms,       gflops\n");
  fprintf(stdout,
          "%*s, %*d, %*.3f, %*f \n",
          23,
	  STRINGIZE_VALUE_OF(VARIANT) ALIGNED_VARIANT,
          6, SIZE,
          9, 1.0e-6 * tmmult,
          12, nflop / tmmult);
#endif

  return EXIT_SUCCESS;
}

static inline long_long_t
timestamp()
{
  struct timespec ts;
  long_long_t timestamp;
  clock_gettime (CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

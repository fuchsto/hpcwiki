#define __GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include <papi.h>

#ifndef SIZE
#define SIZE 1024
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

#define REPEATS 1

#define NEVENTS 2

#if !defined(SCENARIO_NAIVE) && !defined(SCENARIO_OPT)
#pragma error "No scenario selected"
#endif

#ifndef NUM_CACHE_LINES
/* L2 cache: 256 K */
#define CACHE_LINE_SIZE 64
#define NUM_CACHE_LINES ((256 * 1024) / CACHE_LINE_SIZE)
#endif

#define _min_(a,b) (((a) < (b)) ? a : b)

static inline long long timestamp();

void mmult_naive(
  double * A,
  double * B,
  double * C)
{
  int    i, j, k;
  double sum;
  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      sum = 0;
      for (k = 0; k < SIZE; k++) {
        sum += A[i * SIZE + k] * B[k * SIZE + j];
      }
      C[i * SIZE + j] = sum;
    }
  }
}

void mmult_opt(
  double * A,
  double * B,
  double * C)
{
  int    i,  j,  k,
         i0, j0, k0,
         TS;
  double sum;

  /*
   * For 4096 cache lines:
   * TS:         sqrt(4096) / sizeof(double) = 64 / 8 = 8
   * block size: TS x TS = 64 doubles = 256 bytes
   */
//TS = (int)(sqrt(NUM_CACHE_LINES)) / sizeof(double);
  TS = 8;

  assert((SIZE / TS) * TS == SIZE);

  for (i0 = 0; i0 < SIZE; i0 += TS) {
    for (j0 = 0; j0 < SIZE; j0 += TS) {
      for (k0 = 0; k0 < SIZE; k0 += TS) {
        /* Multiply block: */
        for (i = i0; i < _min_(i0 + TS, SIZE); i++) {
          for (j = j0; j < _min_(j0 + TS, SIZE); j++) {
            sum = 0;
            for (k = k0; k < _min_(k0 + TS, SIZE); k++) {
              /* sum += A[i][k] + B[k][j] */
              sum += A[i * SIZE + k] * B[k * SIZE + j];
            }
            /* C[i][j] = sum */
            C[i * SIZE + j] = sum;
          }
        }
      }
    }
  }
}


int main(int argc, char * argv[])
{
  int       events[NEVENTS] = {
              PAPI_TOT_INS,
              PAPI_L2_DCM
            };
  long long papi_values[NEVENTS];
  long long ts_start;
  double    t_sec;
  int       i, j, r;
  double *  pA = (double *)(malloc(SIZE * SIZE * sizeof(double)));
  double *  pB = (double *)(malloc(SIZE * SIZE * sizeof(double)));
  double *  pC = (double *)(malloc(SIZE * SIZE * sizeof(double)));

  double gflop = ((double)(SIZE)/1000.0) *
                 ((double)(SIZE)/1000.0) *
                 ((double)(SIZE)/1000.0) *
                 2.0 * REPEATS;

  if (argc > 1 && strcmp(argv[1], "-h") == 0) {
    printf("    TYPE,  SIZE, REPEAT, G.FLOP,    SEC,  G.FLOPS, G.INS, G.L2_DCM\n");
  }

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
	    pA[i * SIZE + j] = (double)(i) + (double)(j);
	    pB[i * SIZE + j] = (double)(i) + (double)(j);
	  }
  }

  /* Initialize PAPI */
  if (PAPI_start_counters(events, NEVENTS) != PAPI_OK) {
    fprintf(stderr, "Error starting counters\n");
    return EXIT_FAILURE;
  }

  ts_start = timestamp();
  for (r = 0; r < REPEATS; r++) {
#ifdef SCENARIO_NAIVE
    mmult_naive(pA, pB, pC);
#else
    mmult_opt(pA, pB, pC);
#endif
  }
  t_sec = (double)(timestamp() - ts_start) * 1.0e-9;

  /* Finalize PAPI */
  if (PAPI_stop_counters(papi_values, NEVENTS) != PAPI_OK) {
    fprintf(stderr, "Error stopping counters\n");
    return EXIT_FAILURE;
  }

  printf("%*s, %*d, %*d, %*.2f, %*.2f, %*.2f, %*.2f, %*.2f\n",
#ifdef SCENARIO_NAIVE
         8, "naive",
#else
         8, "opt",
#endif
         5, SIZE,
         6, REPEATS,
         6, gflop,
         6, t_sec,
         7, gflop / t_sec,
         6, (double)papi_values[0] * 1.0e-9,
         8, (double)papi_values[1] * 1.0e-9);

  return EXIT_SUCCESS;
}

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime (CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}


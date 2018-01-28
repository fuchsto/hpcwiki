#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#ifndef NTHREADS
# ifdef MAX_THREADS
#  define NTHREADS  (MAX_THREADS)
# else
#  define NTHREADS  4
# endif
#endif

#define BASE_SIZE_MAX  320000000

#ifndef BLOCK_SIZE
#  define BASE_SIZE      BASE_SIZE_MAX
#  define BLOCK_SIZE     (BASE_SIZE / MAX_THREADS)
#endif

#define DOMAIN_SIZE      ((BLOCK_SIZE) * (NTHREADS))

#define DEBUG

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

static inline void printb(long long B[DOMAIN_SIZE], int step) {
  #ifdef DEBUG
    int i;
    printf("step=%5u:\t", step);
    for (i = 0; i < DOMAIN_SIZE; i++) {
        printf("%u\n", B[i]);
    }
    printf("\n");
  #endif
}

int main()
{
  long long i, t;
  long long A[DOMAIN_SIZE];
  long long B[DOMAIN_SIZE];
  long long tstart, tstop;

  #pragma omp parallel for
  for(i = 0; i < DOMAIN_SIZE; i++) {
    A[i] = i;
    // load B into L1 cache ;)
    B[i] = 0;
  }
  // step-1-step
  int step = 2;
  tstart = timestamp();
  for (step = 2; step <= DOMAIN_SIZE; step *= 2) {
    // divide and conquer binary tree
    if (step == 2) {
        #pragma omp parallel for
        // have to copy data from A in first iteration
        for (i = step-1; i < DOMAIN_SIZE; i += step) {
            B[i]   = A[i - step/2] + A[i];
            B[i-1] = A[i - 1];
        }
    } else {
        #pragma omp parallel for
        // now we do not need any data from A anymore
        for (i = step-1; i < DOMAIN_SIZE; i += step) {
            B[i] = B[i - step/2] + B[i];
        }
    }
  }
  step /= 2;
  for (step = step; step > 1; step /= 2) {
    #pragma omp parallel for
    for (i = step-1; i < DOMAIN_SIZE - step/2; i += step) {
        B[i + step/2] = B[i + step/2] + B[i];
    }
  }
  tstop = timestamp();

  printb(B, 0);
  printf("parallel took %0.4f ms\n", 1.0e-6 * (tstop-tstart));

  return EXIT_SUCCESS;
}

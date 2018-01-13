#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <limits.h>
#include <assert.h>

#ifndef NTHREADS
#  define NTHREADS  4
#endif

#define BLOCK_SIZE 60000000
#define SIZE       (NTHREADS * BLOCK_SIZE)

#ifndef MIN
#  define MIN(a,b) (((a)<(b))?(a):(b))
#endif

// static_assert(SIZE < LLONG_MAX, "Size exceeds LLONG_MAX");
static_assert(SIZE < UINT_MAX, "Size exceeds UINT_MAX");

typedef unsigned long input_t;
typedef unsigned long value_t;

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

int main()
{
  value_t t_id;
  value_t i;
  value_t b_begin, b_end, end;
  value_t gauss_sum;
  long long ts_start, duration_p, duration_s;
  value_t block_psum[omp_get_max_threads()];
  value_t temp[omp_get_max_threads()];
  input_t * A;
  value_t * B;

  printf("Partial Sum, N = %lld x %d B = %d MB\n",
         SIZE,
         sizeof(value_t),
         SIZE / 1024 / 1024 * sizeof(value_t));

  A = (input_t *)malloc(sizeof(input_t) * SIZE);
  B = (value_t *)malloc(sizeof(value_t) * SIZE);

  /* Initialize values:
   */
  printf(" -- initialize values ...\n");
  for (i = 0; i < SIZE; i++) {
    A[i] = i;
  }
  B[0] = A[0];

  /* Sequential measurement for reference:
   */
  printf(" -- sequential run ...\n");
  ts_start = timestamp();
  B[0] = A[0];
  for (i = 1; i < SIZE; i++) {
    B[i] = B[i-1] + A[i];
  }
  duration_s = timestamp() - ts_start;

  /* Reset result:
   */
  printf(" -- initialize values ...\n");
  for (i = 0; i < SIZE; i++) {
    B[i] = 0;
  }

  printf(" -- parallel run ...\n");
  ts_start = timestamp();
  #pragma omp parallel num_threads(NTHREADS)
  {
    value_t j;
    value_t sum = 0;
    /* what would happen if we used scheduled(dynamic)?
     */
    #pragma omp for schedule(dynamic, 8)
    for(i = 0; i < SIZE; i++) {
      sum  += A[i];
      B[i]  = sum;
    }
    block_psum[omp_get_thread_num()] = sum;
    #pragma omp barrier
    sum = 0;
    for(j = 0; j < omp_get_thread_num(); j++) {
      sum += block_psum[j];
    }
    #pragma omp for schedule(dynamic, 8)
    for(i = 0; i < SIZE; i++) {
      B[i] += sum;
    }
  }
  duration_p = timestamp() - ts_start;

  printf("------------------------------\n");
  printf("sequential: %.4f ms\n", 1.0e-6 * duration_s);
  printf("parallel:   %.4f ms\n", 1.0e-6 * duration_p);

  for (i = 0; i < MIN(10, SIZE); i++) {
    gauss_sum = ((i + 1) * i) / 2;
    printf("A[%lld]: %lld \t\t B[%lld]: %lld \t exp:%lld\n",
           i, A[i], i, B[i], gauss_sum);
  }
  printf("...\n");
  for (i = SIZE-11; i < SIZE; i++) {
    gauss_sum = ((i + 1) * i) / 2;
    printf("A[%lld]: %lld \t\t B[%lld]: %lld \t exp:%lld\n",
           i, A[i], i, B[i], gauss_sum);
  }

  free(A);
  free(B);

  return EXIT_SUCCESS;
}

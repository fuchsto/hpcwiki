#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <limits.h>
#include <assert.h>

#ifndef MIN
#  define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#  define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* Using compile-time constant to eliminate variance in loop
 * iteration time from differences in branch prediction, loop
 * unrolling, elision/cfolding ...
 * Ugly but robust.
 */
#ifndef MAX_THREADS
#  define MAX_THREADS  4
#endif

static const long long MaxThreads = MIN((MAX_THREADS), 16);
static const long long DomainSize = MIN((MAX_THREADS), 16) * 60000000;

typedef long long index_t;
typedef long long input_t;
typedef long long value_t;
typedef long long tstmp_t;

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
  index_t i;
  value_t b_begin,
          b_end,
          end;
  value_t gauss_sum;
  tstmp_t ts_start,
          duration_p,
          duration_s;
  value_t block_psum[MAX_THREADS];
  value_t temp[MAX_THREADS];
  input_t * A;
  value_t * B;

  printf("Partial Sum, %ld threads, N = %ld x %lu B = %lu MB\n",
         MaxThreads,
         DomainSize,
         sizeof(value_t),
         DomainSize / 1024 / 1024 * sizeof(value_t));

  A = (input_t *)malloc(sizeof(input_t) * DomainSize);
  B = (value_t *)malloc(sizeof(value_t) * DomainSize);

  /* Sequential measurement for reference:
   */
  printf(" == sequential version\n");
  printf(" -- initialize values ...\n");
  for (i = 0; i < DomainSize; i++) {
    A[i] = i;
    B[i] = 0;
  }

  printf(" -- sequential run ...\n");
  ts_start = timestamp();
  B[0] = A[0];
  for (i = 1; i < DomainSize; i++) {
    B[i] = B[i-1] + A[i];
  }
  duration_s = timestamp() - ts_start;


  printf(" == parallel version \n");
  printf(" -- initialize values ...\n");
  /*
   * What might be a bad idea about this?
   */
  for (i = 0; i < DomainSize; i++) {
    B[i] = 0;
  }

  printf(" -- parallel run ...\n");
  ts_start = timestamp();
  #pragma omp parallel num_threads(MAX_THREADS)
  {
    value_t j;
    value_t sum = 0;
    /* what would happen if we used scheduled(dynamic)?
     */
    #pragma omp for schedule(dynamic, 8)
    for(i = 0; i < DomainSize; i++) {
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
    for(i = 0; i < DomainSize; i++) {
      B[i] += sum;
    }
  }
  duration_p = timestamp() - ts_start;

  printf("------------------------------\n");
  printf("sequential: %.4f ms\n", 1.0e-6 * duration_s);
  printf("parallel:   %.4f ms\n", 1.0e-6 * duration_p);

  /* ----------------------------------------------------------------------
   * Why is this broken? There are several things wrong in this code.
   * ----------------------------------------------------------------------
   */

  for (i = 0; i < MIN(10, DomainSize); i++) {
    gauss_sum = ((i + 1) * i) / 2;
    printf("A[%lld]: %lld \t\t B[%lld]: %lld \t exp:%lld\n",
           i, A[i], i, B[i], gauss_sum);
  }
  printf("...\n");
  for (i = DomainSize-11; i < DomainSize; i++) {
    gauss_sum = ((i + 1) * i) / 2;
    printf("A[%lld]: %lld \t\t B[%lld]: %lld \t exp:%lld\n",
           i, A[i], i, B[i], gauss_sum);
  }

  free(A);
  free(B);

  return EXIT_SUCCESS;
}


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
#ifndef MAX_THREADS
#  define BLOCK_SIZE   600000
#endif

static inline void wait_for_confirm() {
  printf("  [ enter to continue ]\n");
  getc(stdin);
}

static const long long MaxThreads = MIN((MAX_THREADS), 16);
static const long long DomainSize = MIN((MAX_THREADS), 16) * (BLOCK_SIZE);

typedef long long index_t;

#if defined(SIEGFRIED_AND_ROOOOY)
typedef long long input_t;
typedef long long value_t;
#else
typedef int       input_t;
typedef int       value_t;
#endif
typedef long long tstmp_t;

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

static void init_arrays(value_t * a, value_t * b, const index_t size) {
  printf(" -- initialize values ...\n");
  for (index_t i = 0; i < size; i++) {
    a[i] = i;
    b[i] = i % 100;
  }
}


int main()
{
  value_t t_id;
  tstmp_t ts_start_p,
          ts_start_s,
          duration_p,
          duration_s;
  input_t * A;
  value_t * B;

  printf("Partial Sum, %ld threads, N = %ld x %lu B = %lu MB\n",
         MaxThreads,
         DomainSize,
         sizeof(value_t),
         DomainSize / 1024 / 1024 * sizeof(value_t));

  A = (input_t *)malloc(sizeof(input_t) * DomainSize);
  B = (value_t *)malloc(sizeof(value_t) * DomainSize);



  /*
   * Sequential measurement for reference:
   */
  printf(" == sequential version\n");
  init_arrays(A, B, DomainSize);

  wait_for_confirm();

  printf(" -- sequential run ...\n");
  ts_start_s = timestamp();

  /*
   * Be super extra sure the wisenheimer compiler
   * does not parallelize these loops automatically:
   */
  #pragma omp parallel
  {
    #pragma omp master
    {
      for (index_t i = 0; i < DomainSize; i++) {
        for (int rep = 0; rep < 10; rep++) {
          B[i] = (int)(B[i] + (i * rep)) - A[i];
        }
      }
    }
    #pragma omp barrier
  }
  duration_s = timestamp() - ts_start_s;




  printf(" == parallel version \n");
  init_arrays(A, B, DomainSize);

  wait_for_confirm();

  printf(" -- parallel run ...\n");
  ts_start_p = timestamp();

  #pragma omp parallel for schedule(static, 1) num_threads(MAX_THREADS)
  for(index_t i = 0; i < DomainSize; i++) {
    for (int rep = 0; rep < 10; rep++) {
      B[i] = (int)(B[i] + (i * rep)) - A[i];
    }
  }
  duration_p = timestamp() - ts_start_p;




  printf("------------------------------\n");
  printf("sequential: %.4f ms\n", 1.0e-6 * duration_s);
  printf("parallel:   %.4f ms\n", 1.0e-6 * duration_p);

  free(A);
  free(B);

  return EXIT_SUCCESS;
}


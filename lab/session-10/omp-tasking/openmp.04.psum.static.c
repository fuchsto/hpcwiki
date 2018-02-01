#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <limits.h>
#include <assert.h>

// #define LOG_DEBUG

#ifndef NTHREADS
#  define NTHREADS     4
#endif

#define BASE_SIZE_MAX  320000000
#define BASE_SIZE      BASE_SIZE_MAX
#define CHUNK_SIZE     8

#ifndef BLOCK_SIZE
#  define BASE_SIZE      BASE_SIZE_MAX
#  define BLOCK_SIZE     (BASE_SIZE / MAX_THREADS)
#endif

#define SIZE           (BLOCK_SIZE * NTHREADS)

#ifndef MIN
#  define MIN(a,b) (((a)<(b))?(a):(b))
#endif

typedef unsigned int input_t;
typedef unsigned long value_t;

static_assert(SIZE < ((value_t)(-1)) / 10,
             "SIZE exceeds value type maximum");

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

  printf("Partial Sum, NT:%d N:%lld (BS:%llu) x %d B = %d MB\n",
         NTHREADS,
         SIZE,
         BLOCK_SIZE,
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
  printf(" == sequential run ...\n");
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

  printf(" == parallel run ...\n");
  ts_start = timestamp();
  #pragma omp parallel shared(A, B, block_psum) \
                       num_threads(NTHREADS)
  {
    value_t j;
    value_t sum = 0;
    /* The chunk size does not have to be specified for blocked
     * tiling, at least according to OpenMP spec:
     *
     *   "Loop iterations are divided into pieces of size chunk
     *    and then statically assigned to threads. If chunk is not
     *    specified, the iterations are evenly (if possible)
     *    divided contiguously among the threads."
     *
     * What would happen if we used scheduled(dynamic)? 
     */
    #pragma omp for schedule(static, BLOCK_SIZE)
    for(i = 0; i < SIZE; i++) {
      sum  += A[i];
      B[i]  = sum;
#ifdef LOG_DEBUG // {{{
      printf("t:%d block[%i]\n", omp_get_thread_num(), i);
#endif // }}}
    }
    block_psum[omp_get_thread_num()] = sum;
    /* wait for all threads to calculate their partial
     * solution:
     */
    #pragma omp barrier 
    /* add last partial sums from previous blocks to
     * initial sum of private block:
     */
    sum = 0;
    for(j = 0; j < omp_get_thread_num(); j++) {
      sum += block_psum[j];
    }
    /* add initial sum of private block to partial sums
     * in private block:
     */
    #pragma omp for schedule(static, BLOCK_SIZE)
    for(i = 0; i < SIZE; i++) {
      B[i] += sum; 
    }
  }
  duration_p = timestamp() - ts_start;
#ifdef LOG_DEBUG // {{{
    for (i = 0; i < NTHREADS; i++) {
      printf("block_psum[%lld]: %lld\n", i, block_psum[i]);
    }
#endif // }}}

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

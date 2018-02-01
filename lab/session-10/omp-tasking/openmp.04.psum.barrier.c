#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <limits.h>
#include <assert.h>

// #define LOG_DEBUG

#ifndef MAX_THREADS
#  define MAX_THREADS  4
#endif

#ifndef BLOCK_SIZE
#  define BASE_SIZE_MAX  320000000
#  define BASE_SIZE      BASE_SIZE_MAX
#  define BLOCK_SIZE     (BASE_SIZE / MAX_THREADS)
#endif

#define DOMAIN_SIZE      ((BLOCK_SIZE) * (MAX_THREADS))
#define CHUNK_SIZE       8

#ifndef MIN
#  define MIN(a,b) (((a)<(b))?(a):(b))
#endif

static_assert(DOMAIN_SIZE < LLONG_MAX, "Size exceeds LLONG_MAX");

typedef unsigned int input_t;
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
  value_t block_psum[MAX_THREADS];
  value_t temp[MAX_THREADS];
  input_t * A;
  value_t * B;

  printf("Partial Sum, N = %lld (BS: %llu) x %d B = %d MB\n",
         DOMAIN_SIZE,
         BLOCK_SIZE,
         sizeof(value_t),
         DOMAIN_SIZE / 1024 / 1024 * sizeof(value_t));

  A = (input_t *)malloc(sizeof(input_t) * DOMAIN_SIZE);
  B = (value_t *)malloc(sizeof(value_t) * DOMAIN_SIZE);

  /* Initialize values:
   */
  printf(" -- initialize values ...\n");
  for (i = 0; i < DOMAIN_SIZE; i++) {
    A[i] = i;
  }
  B[0] = A[0];

  /* Sequential measurement for reference:
   */
  printf(" == sequential run ...\n");
  ts_start = timestamp();
  B[0] = A[0];
  for (i = 1; i < DOMAIN_SIZE; i++) {
    B[i] = B[i-1] + A[i];
  }
  duration_s = timestamp() - ts_start;

  /* Reset result:
   */
  printf(" -- initialize values ...\n");
  for (i = 0; i < DOMAIN_SIZE; i++) {
    B[i] = 0;
  }

  printf(" == parallel run ...\n");
  ts_start = timestamp();
  /* - assign a sub-array (block) of input array A to every
   *   thread.
   * - each thread calculates prefix-sums in his block.
   * - the last value in every block has to be added to all
   *   following values.
   */
  #pragma omp parallel shared(A, B, block_psum, temp) \
                       private(t_id, i, b_begin, b_end, end) \
                       num_threads(MAX_THREADS)
  {
    value_t sum = 0;
    t_id        = omp_get_thread_num();
    b_begin     = BLOCK_SIZE * t_id;
    b_end       = b_begin + BLOCK_SIZE;
    B[b_begin]  = A[b_begin];
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
    for (i = 0; i < DOMAIN_SIZE; i++) {
#ifdef LOG_DEBUG // {{{
      printf("t:%d block[%i]\n", t_id, i);
#endif // }}}
      sum += A[i];
      B[i] = sum;
    }
    /* save last partial sum from private block:
     */
    block_psum[t_id] = B[b_end-1];
    /* wait for all threads to calculate their partial
     * solution:
     */
    #pragma omp barrier
    /* calculate prefix sum for the array that was made from
     * last elements of each of the previous sub-arrays:
     */
    for (i = 1; i < MAX_THREADS; i <<= 1) {
      /* i = 1, 2, 4, 8, ... nthreads */
      if (t_id >= i) {
        temp[t_id] = block_psum[t_id] + block_psum[t_id - i];
      }
      #pragma omp barrier
      #pragma omp single
      {
        memcpy(block_psum+1, temp+1, sizeof(value_t) * (MAX_THREADS-1));
      }
    }
    end = MIN(b_end, DOMAIN_SIZE);
    for (i = b_begin; i < end; i++) {
      B[i] += block_psum[t_id] - B[end-1];
    }
  }
  duration_p = timestamp() - ts_start;
#ifdef LOG_DEBUG // {{{
    for (i = 0; i < MAX_THREADS; i++) {
      printf("block_psum[%lld]: %lld\n", i, block_psum[i]);
    }
#endif // }}}

  printf("------------------------------\n");
  printf("sequential: %.4f ms\n", 1.0e-6 * duration_s);
  printf("parallel:   %.4f ms\n", 1.0e-6 * duration_p);

  for (i = 0; i < MIN(10, DOMAIN_SIZE); i++) {
    gauss_sum = ((i + 1) * i) / 2;
    printf("A[%lld]: %lld \t\t B[%lld]: %lld \t exp:%lld\n",
           i, A[i], i, B[i], gauss_sum);
  }
  printf("...\n");
  for (i = DOMAIN_SIZE-11; i < DOMAIN_SIZE; i++) {
    gauss_sum = ((i + 1) * i) / 2;
    printf("A[%lld]: %lld \t\t B[%lld]: %lld \t exp:%lld\n",
           i, A[i], i, B[i], gauss_sum);
  }

  return EXIT_SUCCESS;
}

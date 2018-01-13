#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <malloc.h>

#ifndef NTHREADS
#  define NTHREADS     4
#endif

#define BASE_SIZE_MAX  ((1 << 20) * 512)
#define BASE_SIZE      BASE_SIZE_MAX
#define BLOCK_SIZE     (BASE_SIZE / NTHREADS)
#define SIZE           (BLOCK_SIZE * NTHREADS)
#define CHUNK_SIZE     4
#define ALIGNMENT      0x40

typedef int value_t;

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

void run_parallel_for(
    value_t * values,
    int chunk_size)
{
  int       i;
  long long ts_start,
            duration;
  ts_start = timestamp();
  #pragma omp parallel for num_threads(NTHREADS) \
                           shared(values) \
                           schedule(static, chunk_size)
  for (i = 0; i < SIZE; i++) {
    values[i] = -values[i];
  }
  duration = timestamp() - ts_start;
  printf("CS:%*d, time: %.4f\n", 7, chunk_size, 1.0e-6 * duration);
}

int main()
{
  int t_id;
  long long i;
  value_t * values;

  values = (value_t *)memalign(ALIGNMENT, SIZE * sizeof(value_t));

  printf("Simple parallel for, N:%d NT:%lld\n",
         SIZE, NTHREADS);

  run_parallel_for(values, 1);
  run_parallel_for(values, 2);
  run_parallel_for(values, 4);
  run_parallel_for(values, 8);
  run_parallel_for(values, 16);
  run_parallel_for(values, 64);
  run_parallel_for(values, 256);
  run_parallel_for(values, 0x1000);
  run_parallel_for(values, 0x2000);
  run_parallel_for(values, 0x3000);
  run_parallel_for(values, 0x4000);

#if 0
  ts_start = timestamp();
  #pragma omp parallel for num_threads(NTHREADS) \
                           schedule(static)
  for (i = 0; i < SIZE; i++) {
    values[i] = -values[i];
  }
  duration = timestamp() - ts_start;
  printf("CS: -, time: %.4f\n", 1.0e-6 * duration);
#endif

  free(values);

  return 0;
}

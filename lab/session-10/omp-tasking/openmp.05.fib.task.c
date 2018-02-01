
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>


typedef long long value_t;
typedef long long index_t;

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}


static void fib__tpar(
    value_t * const fib,
    index_t         size)
{
#pragma omp parallel
  {
#pragma omp single
    {
#pragma omp task depend(out : fib[0], \
                              fib[1], \
                              fib[2])
      {
        fib[0] = 0;
        fib[1] = 1;
        fib[2] = 1;
      }

      for (int i = 3; i < size; i++) {
        int half = i / 2;
#pragma omp task depend(in  : fib[half], \
                              fib[half + 1]) \
                 depend(out : fib[i])
        {
          if (i % 2) {
            fib[i] = fib[half] * fib[half] + fib[half + 1] * fib[half + 1];
          } else {
            fib[i] = (2 * fib[half + 1] - fib[half]) * fib[half];
          }
        }
      }
    }
  }
}

int main(int argc, char * argv[])
{
  long long ts_start,
            duration_p;

  index_t size = 128;
  int     nrep = 100000;

  if (argc >= 2) {
    size = atol(argv[1]);
  } else {
    size = 1024 * 1024;
  }
  if (argc >= 3) {
    nrep = atoi(argv[2]);
  } else {
    nrep = 1024 * 1024;
  }

  printf("------------------------------\n");
  printf("fib size: %d\n", size);
  printf("repeat: : %d\n", nrep);
  printf("------------------------------\n");

  value_t * fib = calloc(size, sizeof(value_t));

  ts_start = timestamp();
  for (int r = 0; r < nrep; r++) {
    fib__tpar(fib, size);
  }
  duration_p = timestamp() - ts_start;

  for (int i = 0; i < 10; i++) {
    printf("fib[%*.lld] = %lld\n", 4, i, fib[i]);
  }
  int step = 2;
  for (int i = 10; i < size; i += step, step *= 2) {
    printf("fib[%*.lld] = %lld\n", 4, i, fib[i]);
  }

  printf("------------------------------\n");
  printf("parallel:   %.4f ms\n", 1.0e-6 * duration_p);

  return EXIT_SUCCESS;
}


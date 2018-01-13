#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#ifndef NTHREADS
#  define NTHREADS  4
#endif

#define CHUNK_SIZE    2
#define NITER_DEFAULT 800000

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

int isprime(int p)
{
  int d;
  for (d = 2; d < p; d = d + 1)
    if (p % d == 0)
      return 0;
  return 1;
}

int main(int argc, char * argv[])
{
  int       n_iter = NITER_DEFAULT;
  int       i;
  int       primes;
  long long ts_start;
  long long duration;

  if (argc >= 2) {
    n_iter = atoi(argv[1]);
  }
  printf("Prime Sieve\n");
  printf("    limit:  %d\n", n_iter);
  printf("    chunk:  %d\n", CHUNK_SIZE);

  printf(" -- schedule(static)");
  fflush(stdout);
  primes = 0;
  ts_start = timestamp();
  #pragma omp parallel for reduction(+:primes) \
                           schedule(static) \
                           num_threads(NTHREADS)
  for (i = 2; i < n_iter; i++) {
    if (isprime(i) == 1) {
      primes++;
    }
  }
  duration = timestamp() - ts_start;
  printf("     primes: %d   time: %0.4f ms\n",
         primes,
         1.0e-6 * duration);

  printf(" -- schedule(dynamic)");
  fflush(stdout);
  primes = 0;
  ts_start = timestamp();
  #pragma omp parallel for reduction(+:primes) \
                           schedule(dynamic, CHUNK_SIZE) \
                           num_threads(NTHREADS)
  for (i = 2; i < n_iter; i++) {
    if (isprime(i) == 1) {
      primes++;
    }
  }
  duration = timestamp() - ts_start;
  printf("    primes: %d   time: %0.4f ms\n",
         primes,
         1.0e-6 * duration);

  printf(" -- schedule(guided) ");
  fflush(stdout);
  primes = 0;
  ts_start = timestamp();
  #pragma omp parallel for reduction(+:primes) \
                           schedule(guided, CHUNK_SIZE) \
                           num_threads(NTHREADS)
  for (i = 2; i < n_iter; i++) {
    if (isprime(i) == 1) {
      primes++;
    }
  }
  duration = timestamp() - ts_start;
  printf("    primes: %d   time: %0.4f ms\n",
         primes,
         1.0e-6 * duration);

  printf(" -- schedule(auto) ");
  fflush(stdout);
  primes = 0;
  ts_start = timestamp();
  #pragma omp parallel for reduction(+:primes) \
                           schedule(auto) \
                           num_threads(NTHREADS)
  for (i = 2; i < n_iter; i++) {
    if (isprime(i) == 1) {
      primes++;
    }
  }
  duration = timestamp() - ts_start;
  printf("    primes: %d   time: %0.4f ms\n",
         primes,
         1.0e-6 * duration);

  return 0;
}

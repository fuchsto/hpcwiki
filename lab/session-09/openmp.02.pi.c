#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#ifndef NTHREADS
#  define NTHREADS  4
#endif

#define CHUNK_SIZE    8
#define NITER_DEFAULT 2000000000;

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

typedef struct measurement_t {
  double    value;
  long long ns;
} measurement;

measurement pi_sequential(long long niter) {
  long long  i;
  double     pi_4th_acc = 0.0;
  long long  ts_start, ts_end;
  measurement m;

  ts_start = timestamp();
  for (i = 0; i < niter; i++) {
    if (i % 2 == 0) {
      pi_4th_acc += 1.0 / (double)(2 * i + 1);
    } else {
      pi_4th_acc -= 1.0 / (double)(2 * i + 1);
    }
  }
  ts_end = timestamp();

  m.value = pi_4th_acc * 4;
  m.ns    = ts_end - ts_start;
  return m;
}

measurement pi_parallel(long long niter) {
  long long  i;
  double     pi_4th_acc = 0.0;
  long long  ts_start, ts_end;
  measurement m;

  ts_start = timestamp();
  /* Reduction:
   * for i : 0..niter
   *   pi_4th_acc += <value of pi_4th_acc at end of iteration>
   */
  #pragma omp parallel for schedule(static, CHUNK_SIZE) reduction(+:pi_4th_acc)
  for (i = 0; i < niter; i++) {
    if (i % 2 == 0) {
      pi_4th_acc += 1.0 / (double)(2 * i + 1);
    } else {
      pi_4th_acc -= 1.0 / (double)(2 * i + 1);
    }
  }
  ts_end = timestamp();

  m.value = pi_4th_acc * 4;
  m.ns    = ts_end - ts_start;
  return m;
}

int main(int argc, char * argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: openmp.02.pi.a s|p [niter]\n");
    return EXIT_FAILURE;
  }
  char        mode  = argv[1][0];
  long long   niter = NITER_DEFAULT;
  measurement m;

  printf("Calculation of Pi (reduction)\n");

  if (argc >= 3) {
    niter = atoi(argv[2]);
  }
  if (mode == 's') {
    printf("sequential mode, %d iterations\n", niter);
    m = pi_sequential(niter);
  }
  else if (mode == 'p') {
    printf("parallel mode, %d iterations\n", niter);
    m = pi_parallel(niter);
  }

  printf("-----------------------------------\n");
  printf("result:   %.10f\n", m.value);
  printf("expected: %.10f (last dec. place may differ)\n", M_PI);
  printf("delta:    %.10f\n", M_PI - m.value);
  printf("time:     %f ms\n", 1.0e-6 * m.ns);

  return EXIT_SUCCESS;
}

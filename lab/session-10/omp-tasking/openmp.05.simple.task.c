
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
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


static value_t calculate__tpar()
{
  int x, y, z;
#pragma omp parallel
  {
#pragma omp single
    {
#pragma omp task depend(out: x)
      {
        x = 100;
        usleep(30 * 1000);
      }
#pragma omp task depend(out: y)
      {
        y = 101;
        usleep(30 * 1000);
      }
#pragma omp task depend(in  : x,y) \
                 depend(out : z)
      {
        z = x + y;
        usleep(30 * 1000);
      }
    }
  }
  return z;
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

  value_t result = calculate__tpar();

  printf("------------------------------\n");
  printf("result: %lld\n", result);

  return EXIT_SUCCESS;
}


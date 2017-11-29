#define __GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef _NITER
#  define NITER _NITER
#else
#  define NITER 100000000
#endif

/* Number of operations per iteration */
#define WIDTH 12

#ifdef _BENCH_TYPE_INT
#  define _TYPE_           int
#  define _TYPE_STR_       "int"
#endif
#ifdef _BENCH_TYPE_DOUBLE
#  define _TYPE_           double
#  define _TYPE_STR_       "double"
#endif
#ifdef _BENCH_TYPE_FLOAT
#  define _TYPE_           float
#  define _TYPE_STR_       "float"
#endif

#ifdef _BENCH_OP_ADD
#  define _OP_             +
#  define _START_          1
#  define _FACTOR_         23
#  define _OP_STR_         "add"
#endif
#ifdef _BENCH_OP_DIV
#  define _OP_             /
#  define _START_          3424234.0
#  define _FACTOR_         1.00000001
#  define _OP_STR_         "div"
#endif
#ifdef _BENCH_OP_MUL
#  define _OP_             *
#  define _START_          0.1
#  define _FACTOR_         1.00000001
#  define _OP_STR_         "mul"
#endif

void op_pipe(
  _TYPE_ d,
  _TYPE_ factor);

void op_nopipe(
  _TYPE_ d,
  _TYPE_ factor);

#define long_long_t long long
#define long_long_f "%lld"

static inline long_long_t timestamp();


int main(int argc, char * argv[])
{
  if (argc > 1 && strcmp(argv[1], "-h") == 0) {
    /* Print CSV header */
    printf("'scenario',"
           "'type',"
           "'op',"
           "'num op',"
           "'result',"
           "'nseconds'"
           "\n");
  }
  /* No Pipeline Scenario */
  op_nopipe(
    _START_,
    _FACTOR_);

  /* Pipeline Scenario */
  op_pipe(
    _START_,
    _FACTOR_);

  return EXIT_SUCCESS;
}

void op_pipe(
  _TYPE_ d,
  _TYPE_ factor)
{
  long_long_t    i;
  long_long_t    ts_start,
               ts_end;
  long_long_t    nseconds;
  _TYPE_       res;
  _TYPE_       d1, d2, d3,  d4,  d5,  d6,
               d7, d8, d9, d10, d11, d12;

  d1 = d2 = d3 =  d4 =  d5 =  d6 = d7 =
       d8 = d9 = d10 = d11 = d12 = d;

  ts_start = timestamp();
  for (i = 0; i < NITER; i++) {
    d1  =  d1 _OP_ factor;
    d2  =  d2 _OP_ factor;
    d3  =  d3 _OP_ factor;
    d4  =  d4 _OP_ factor;
    d5  =  d5 _OP_ factor;
    d6  =  d6 _OP_ factor;
    d7  =  d7 _OP_ factor;
    d8  =  d8 _OP_ factor;
    d9  =  d9 _OP_ factor;
    d10 = d10 _OP_ factor;
    d11 = d11 _OP_ factor;
    d12 = d12 _OP_ factor;
  }
  ts_end = timestamp();

  res  = 0.0;
  res += d1 + d2 + d3 +  d4 +  d5 +  d6;
  res += d7 + d8 + d9 + d10 + d11 + d12;

  nseconds = (ts_end - ts_start);

  printf("'pipelined',"
         "'%s',"
         "'%s',"
         "'" long_long_f "',"
         "'%g',"
         "'" long_long_f "'\n",
         _TYPE_STR_,
         _OP_STR_,
         (long_long_t)NITER * WIDTH,
         (double)res,
         nseconds
  );
}

void op_nopipe(
  _TYPE_ d,
  _TYPE_ factor)
{
  long_long_t  i;
  long_long_t  ts_start,
               ts_end;
  long_long_t  nseconds;
  _TYPE_       res;
  _TYPE_       d1, d2, d3,  d4,  d5,  d6,
               d7, d8, d9, d10, d11, d12;

  d1 = d2 = d3 =  d4 =  d5 =  d6 = d7 =
       d8 = d9 = d10 = d11 = d12 = d;

  ts_start = timestamp();
  for (i = 0; i < NITER; i++) {
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
    d1 = d1 _OP_ factor;
  }
  ts_end = timestamp();

  res  = 0.0;
  res += d1 + d1 + d1 + d1 + d1 + d1;
  res += d1 + d1 + d1 + d1 + d1 + d1;

  nseconds = (ts_end - ts_start);

  printf("'not pipelined',"
         "'%s',"
         "'%s',"
         "'" long_long_f "',"
         "'%g',"
         "'" long_long_f "'\n",
         _TYPE_STR_,
         _OP_STR_,
         (long_long_t)NITER * WIDTH,
         (double)res,
         nseconds
  );
}

static inline long_long_t timestamp()
{
  /* Note:
   * Don't use gettimeofday() for measuring time
   * in benchmarks.
   */
  struct timespec ts;
  long_long_t timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}


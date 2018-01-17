#define __GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#ifndef REPEAT
#define REPEAT 100
#endif

#ifndef _NTHREADS
#define _NTHREADS 4
#endif

#define timestamp_t long long

static inline timestamp_t timestamp();

int main(int argc, char* argv[])
{
    int         i, r;
    int         v_size;
    long long   mflop;
    double      mflops;
    double      t_ns;
    timestamp_t tstart, tstop;
    double *    vec_a;
    double *    vec_b;
    double *    vec_c;
    double *    vec_d;
  
    /*
     * Default vector size:
     */
    v_size = 0x4000;

    if ((argc > 1 && strcmp(argv[1], "-h") == 0) ||
        (argc > 3 && strcmp(argv[3], "-h") == 0)) {
      fprintf(stderr, "%*s,%*s,%*s,%*s,%*s,%*s,%*s\n",
              8,  "type",
              4,  "n",
              12, "vsize",
              8,  "repeat",
              12, "t.ms",
              12, "mflop",
              12, "mflop/s");
    }
    if (argc > 2 && strcmp(argv[1], "-v") == 0) {
      v_size = atoi(argv[2]);
    }
    if (argc > 3 && strcmp(argv[2], "-v") == 0) {
      v_size = atoi(argv[3]);
    }

    vec_a = (double*)(malloc(sizeof(double) * v_size));
    vec_b = (double*)(malloc(sizeof(double) * v_size));
    vec_c = (double*)(malloc(sizeof(double) * v_size));
    vec_d = (double*)(malloc(sizeof(double) * v_size));

    /*
     * Initialize vectors:
     */
#ifdef _PARALLEL_INIT
    #pragma omp parallel for private(i) num_threads(_NTHREADS)
    for (i = 0; i < v_size; i++) {
      vec_a[i] = 0.0;
      vec_b[i] = (double)(v_size - i);
      vec_c[i] = (double)(i);
      vec_d[i] = (double)(v_size * 100);
    }
#else
    for (i = 0; i < v_size; i++) {
      vec_a[i] = 0.0;
      vec_b[i] = (double)(v_size - i);
      vec_c[i] = (double)(i);
      vec_d[i] = (double)(v_size * 100);
    }
#endif
    
    /*
     * Run vector triad operation according to Schönauer:
     *   a[i] = b[i] + ( c[i] * d[i] )
     */
    tstart = timestamp();
    for (r = 0; r < REPEAT; r++) {
      #pragma omp parallel for private(i,r) num_threads(_NTHREADS)
      for (i = 0; i < v_size; i++) {
        vec_a[i] = vec_b[i] + vec_c[i] * vec_d[i];
      }
    }
    tstop  = timestamp();
    t_ns   = (double)(tstop - tstart);
    mflop  = ((long long)(v_size) * REPEAT * 2) / 1e6;
    mflops = (double)(mflop) / (1.0e-9 * t_ns);

    fprintf(stderr, "%*s,%*d,%*d,%*d,%*.3f,%*lld,%*.3f\n",
#ifdef _PARALLEL_INIT
            8,  "pi",
#else
            8,  "si",
#endif
            4,  _NTHREADS,
            12, v_size,
            8,  REPEAT,
            12, 1.0e-6 * t_ns,
            12, mflop,
            12, mflops);

    free(vec_a);
    free(vec_b);
    free(vec_c);
    free(vec_d);

    return EXIT_SUCCESS;
}

static inline timestamp_t timestamp()
{
  struct timespec ts;
  timestamp_t timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}


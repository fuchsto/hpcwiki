#ifndef AUXIL_H__INCLUDED
#define AUXIL_H__INCLUDED

#include <stdio.h>
#include <string.h>


#define PLEASE_PLEASE_INLINE __attribute__((always_inline)) static inline 


#define swap_buffers(a, b)                                                     \
  do {                                                                         \
    PGMPixel *tmp = a;                                                         \
    (a) = (b);                                                                 \
    (b) = tmp;                                                                 \
  } while (0)

#define LOGT(msg, ...)                                                         \
  do {                                                                         \
    char buf[200];                                                             \
    sprintf(buf, "thread:%d -- ", omp_get_thread_num());                       \
    sprintf(buf + strlen(buf), msg, __VA_ARGS__);                              \
    printf("%s\n", buf);                                                       \
  } while (0)

#define LOGMT(msg, ...)                                                        \
  do {                                                                         \
    if (omp_get_thread_num() == 0) {                                           \
      LOGT(msg, __VA_ARGS__);                                                  \
    }                                                                          \
  } while (0)

#define LOG_PROGRESS(ncurr, ntotal, nsteps)                                    \
  do {                                                                         \
    if (ntotal > nsteps && (ncurr == 0 || ncurr % (ntotal / nsteps) == 0)) {        \
      LOGMT("%d%%", iter / (ntotal / nsteps) * (100 / nsteps));                \
    }                                                                          \
  } while (0)

#endif // AUXIL_H__INCLUDED

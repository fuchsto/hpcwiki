#ifndef HISTOGRAM_H__INCLUDED
#define HISTOGRAM_H__INCLUDED

#include "pgm.h"

#include <stdlib.h>



typedef struct Histogram_s {
  size_t * key_counts;
  int      num_keys;
} Histogram;

Histogram histogram_new(PGMData image);
void      histogram_delete(Histogram * hist);


#endif // HISTOGRAM_H__INCLUDED

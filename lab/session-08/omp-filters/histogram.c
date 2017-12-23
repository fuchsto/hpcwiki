
#include "pgm.h"
#include "histogram.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>


Histogram histogram_new(PGMData image)
{
  Histogram hist;
  hist.num_keys   = image.thld;
  hist.key_counts = (size_t *)(
                      aligned_alloc(512, hist.num_keys * sizeof(size_t)));

  memset(hist.key_counts, 0,
         hist.num_keys * sizeof(size_t));

  #pragma omp parallel for schedule(static)
  for (int row = 0; row < image.rows; row++) {
    for (int col = 0; col < image.cols; col++) {
      int data_offs  = (image.cols * row) + col;
      hist.key_counts[ (int)nearbyint(image.data[data_offs]) ]++;
    }
  }

  return hist;
}


void histogram_delete(const Histogram hist)
{
  free(hist.key_counts);
}



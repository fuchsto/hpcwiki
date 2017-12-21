
#include "convolute.h"
#include "auxil.h"
#include "pgm.h"

#include <string.h>
#include <stdlib.h>

#include <omp.h>


void convolute(PGMData image, const CellOperator op, int niterations)
{
  PGMPixel * buffer_back  = (PGMPixel *)(
                              aligned_alloc(
                                512,
                                image.cols * image.rows * sizeof(PGMPixel)));

  PGMPixel * buffer_front = image.data;

  for (int iter = 0; iter < niterations; iter++)
  {
    LOG_PROGRESS(iter, niterations, 20);

    #pragma omp parallel for schedule(static)
    for (int row = 1; row < image.rows - 1; row++) {
      for (int col = 1; col < image.cols - 1; col++) {
        // Apply stencil: write to front buffer, read from back buffer:
        op(buffer_front,
           buffer_back,
           image.cols,
           image.rows,
           col,
           row);
      }
    }
    // omp for has implicit barrier here
    swap_buffers(buffer_front, buffer_back);
  }
}


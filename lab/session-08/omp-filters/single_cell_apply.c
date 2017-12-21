
#include "single_cell_apply.h"
#include "auxil.h"
#include "pgm.h"

#include <string.h>
#include <stdlib.h>

#include <omp.h>


void single_cell_apply(PGMData image, const CellUnaryOperator op)
{
  #pragma omp parallel for schedule(static)
  for (int row = 0; row < image.rows; row++) {
    for (int col = 0; col < image.cols; col++) {
      // Apply stencil: write to front buffer, read from back buffer:
      int data_offs         = (image.cols * row) + col;
      image.data[data_offs] = op(image.data[data_offs]);
    }
  }
}


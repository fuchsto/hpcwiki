
#include "blend.h"

#include "auxil.h"
#include "pgm.h"

#include <omp.h>



void blend(
  PGMData                  dst_image,
  PGMData                  src_image,
  const CellBinaryOperator op)
{
  #pragma omp parallel for schedule(static)
  for (int row = 0; row < dst_image.rows; row++) {
    for (int col = 0; col < dst_image.cols; col++) {
      int data_offs             = (dst_image.cols * row) + col;
      dst_image.data[data_offs] = op(dst_image.data[data_offs],
                                     src_image.data[data_offs],
                                     dst_image,
                                     src_image);
    }
  }
}





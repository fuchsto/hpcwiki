#ifndef BLEND_H__INCLUDED
#define BLEND_H__INCLUDED

#include "pgm.h"
#include "auxil.h"

#include <math.h>


typedef PGMPixel (*CellBinaryOperator)(
                     PGMPixel      a,
                     PGMPixel      b,
                     const PGMData image_a,
                     const PGMData image_b);


PLEASE_PLEASE_INLINE
PGMPixel op_difference(
  PGMPixel      a,
  PGMPixel      b,
  PGMData const image_a,
  PGMData const image_b)
{
  return fabs(a - b);
}


void blend(
  PGMData                  dst_image,
  PGMData  const           src_image,
  const CellBinaryOperator op);


#endif // BLEND_H__INCLUDED


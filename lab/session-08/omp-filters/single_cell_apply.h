#ifndef SINGLE_CELL_APPLY_H__INCLUDED
#define SINGLE_CELL_APPLY_H__INCLUDED

#include "auxil.h"
#include "pgm.h"


typedef PGMPixel (*CellUnaryOperator)(PGMPixel pxval);

PLEASE_PLEASE_INLINE
PGMPixel op_invert(PGMPixel pxval, PGMData image)
{
  return image.thld - pxval;
}


void single_cell_apply(PGMData image, const CellUnaryOperator op);


#endif // SINGLE_CELL_APPLY_H__INCLUDED


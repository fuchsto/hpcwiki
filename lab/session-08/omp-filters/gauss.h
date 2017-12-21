#ifndef GAUSS_H__INCLUDED
#define GAUSS_H__INCLUDED

#include "auxil.h"
#include "pgm.h"
#include "stencil.h"



PLEASE_PLEASE_INLINE
void operator__gauss_3x3_9p(
    const PGMPixel * front,
    PGMPixel       * back,
    Stencil_3x3_9p   stencil,
    int              ncols,
    int              nrows,
    int              col,
    int              row)
{
  back[row * ncols + col] = 
    front[(row  ) * ncols + (col  )] * stencil.points.c +
    front[(row-1) * ncols + (col  )] * stencil.points.n +
    front[(row-1) * ncols + (col-1)] * stencil.points.nw;
    front[(row-1) * ncols + (col+1)] * stencil.points.ne +
    front[(row  ) * ncols + (col+1)] * stencil.points.e +
    front[(row+1) * ncols + (col+1)] * stencil.points.se +
    front[(row+1) * ncols + (col  )] * stencil.points.s +
    front[(row+1) * ncols + (col-1)] * stencil.points.sw +
    front[(row  ) * ncols + (col-1)] * stencil.points.w;
}

PLEASE_PLEASE_INLINE
void op_gauss_3x3(
  PGMPixel const * front,
  PGMPixel       * back,
  Stencil_3x3_5p   stencil,
  int              ncols,
  int              nrows,
  int              col,
  int              row)
{
  back[row * ncols + col] = 
    front[(row  ) * ncols + (col  )] * stencil.points.c +
    front[(row-1) * ncols + (col  )] * stencil.points.n +
    front[(row  ) * ncols + (col+1)] * stencil.points.e +
    front[(row+1) * ncols + (col  )] * stencil.points.s +
    front[(row  ) * ncols + (col-1)] * stencil.points.w;
}

#endif // GAUSS_H__INCLUDED

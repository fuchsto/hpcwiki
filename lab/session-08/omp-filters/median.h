#ifndef MEDIAN_H__INCLUDED
#define MEDIAN_H__INCLUDED



__attribute__((always_inline))
static inline void operator__median_3x3_5p(
    const PGMPixel * front,
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

__attribute__((always_inline))
static inline void operator__median_3x3_9p(
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


#endif // MEDIAN_H__INCLUDED

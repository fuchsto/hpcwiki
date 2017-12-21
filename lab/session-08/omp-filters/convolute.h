#ifndef CONVOLUTE_H__INCLUDED
#define CONVOLUTE_H__INCLUDED

#include "pgm.h"
#include "stencil.h"

typedef void (*CellOperator)(
                PGMPixel const * front,
                PGMPixel       * back,
                int              ncols,
                int              nrows,
                int              col,
                int              row);

void convolute(PGMData data, const CellOperator op, int niterations);

#endif // CONVOLUTE_H__INCLUDED


#include "convolute.h"
// #include "normalize.h"
// #include "erode.h"
// #include "dilate.h"

#include "pgm.h"
#include "gauss.h"
#include "auxil.h"
#include "stencil.h"
#include "blend.h"
#include "histogram.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static const Stencil_3x3_5p
  gauss_3x3_stencil_A { 0.4,
                        0.15, 0.15, 0.15, 0.15 };

static const Stencil_3x3_5p
  gauss_3x3_stencil_B { 0.68,
                        0.08, 0.08, 0.08, 0.08 };


PLEASE_PLEASE_INLINE
void op_gauss_3x3_A(
  PGMPixel const * front,
  PGMPixel       * back,
  int              ncols,
  int              nrows,
  int              col,
  int              row)
{
  op_gauss_3x3(front, back, gauss_3x3_stencil_A, ncols, nrows, col, row);
}

PLEASE_PLEASE_INLINE
void op_gauss_3x3_B(
  PGMPixel const * front,
  PGMPixel       * back,
  int              ncols,
  int              nrows,
  int              col,
  int              row)
{
  op_gauss_3x3(front, back, gauss_3x3_stencil_B, ncols, nrows, col, row);
}




int main(int argc, char * argv[])
{
  // Yada yada boilerplate:
  // ------------------------------------------------------------------------
  //
  if (argc == 1) {
    fprintf(stderr, "usage: omp-filter.bin <PGM input file>\n");
    return EXIT_FAILURE;
  }
  
  int niterations_A = 750;
  int niterations_B = 130;

  if (argc >= 3) {
    int niterations_A = atoi(argv[2]);
  }
  if (argc >= 4) {
    int niterations_B = atoi(argv[3]);
  }

  // Read input image:
  // ------------------------------------------------------------------------
  //
  const char * input_filename = argv[1];
  PGMData    * pgm_orig       = pgm_read(input_filename);
  // Keep clone of original image data:
  PGMData    * pgm_data_A     = pgm_clone(pgm_orig);
  PGMData    * pgm_data_B     = pgm_clone(pgm_orig);

  // Image processing steps:
  // ------------------------------------------------------------------------
  //
  //        .-----> [ Gaussian A ]--.
  //       /  \                     [ Difference ]---.
  // in --<    '--> [ Gaussian B ]--'                |
  //       \                                         [ Exclude ]--> out
  //        \                                        |
  //         `----> [   Median   ]-------------------'
  //

  // Gaussian A:
  //
  convolute(
      *pgm_data_A,
      op_gauss_3x3_A,
      niterations_A);
  pgm_write(pgm_data_A, "result_conv_A.pgm");

  // Gaussian B:
  //
  convolute(
      *pgm_data_B,
      op_gauss_3x3_B,
      niterations_B);
  pgm_write(pgm_data_A, "result_conv_B.pgm");

  // A -= B
  //
  blend(
      *pgm_data_A,
      *pgm_data_B,
      op_difference);
  pgm_write(pgm_data_A, "result_blend_AB.pgm");

  // Input -= A
  //
  blend(
      *pgm_orig,
      *pgm_data_A,
      op_difference);
  pgm_write(pgm_orig,  "result_blend_OA.pgm");


  // Write out result image:
  // ------------------------------------------------------------------------
  //
  pgm_write(pgm_data_A, "result_A.pgm");
  pgm_write(pgm_orig,   "result_O.pgm");

  return EXIT_SUCCESS;
}



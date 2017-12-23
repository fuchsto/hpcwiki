#ifndef STENCIL_H__INCLUDED
#define STENCIL_H__INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

typedef union _Stencil_3x3_5p {
  struct {
    double c;
    double n;
    double e;
    double s;
    double w;
  } points;
  double weights[5];
} Stencil_3x3_5p;

typedef union _Stencil_3x3_9p {
  struct {
    double c;
    double n;
    double ne;
    double e;
    double se;
    double s;
    double sw;
    double w;
    double nw;
  } points;
  double weights[9];
} Stencil_3x3_9p;

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // STENCIL_H__INCLUDED

#ifndef PGM_H__INCLUDED
#define PGM_H__INCLUDED



#if defined(__cplusplus)
extern "C" {
#endif


typedef double PGMPixel;

typedef struct _PGMData {
  int        rows;
  int        cols;
  int        thld;
  PGMPixel * data; // load image data as floats, write back as int8_t
}
PGMData;


PGMData * pgm_new(
  int             rows,
  int             cols,
  int             thld);

PGMData * pgm_clone(
  const PGMData * pgm_src);

void pgm_delete(
  PGMData       * pgm);

PGMData * pgm_read(
  const char    * file_name);

int pgm_write(
  const PGMData * pgm,
  const char    * file_name);


#if defined(__cplusplus)
} // extern "C"
#endif

#endif // PGM_H__INCLUDED

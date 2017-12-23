#include "pgm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define wno_unused_var(v) (void)(v)

static void pgm_file_skip_comments(FILE * fp);

/*
 * PGM file format specification:
 * http://netpbm.sourceforge.net/doc/pgm.html
 */

#define HI(num) (((num) & 0x0000FF00) >> 8)
#define LO(num)  ((num) & 0x000000FF)

PGMData * pgm_new(
  int rows,
  int cols,
  int thld)
{
  PGMData  * pgm;
  PGMPixel * data;
  int        i;

  data = (PGMPixel *)(aligned_alloc(512, sizeof(PGMPixel) * rows * cols));
  if (NULL == data) {
    perror("pgm_new failed");
    exit(EXIT_FAILURE);
  }

  pgm = (PGMData *)(malloc(sizeof(PGMData)));
  pgm->data = data;
  pgm->rows = rows;
  pgm->cols = cols;
  pgm->thld = thld;

  return pgm;
}

PGMData * pgm_clone(
  const PGMData * pgm_src)
{
  PGMData * pgm_clone = pgm_new(pgm_src->rows, pgm_src->cols, pgm_src->thld);

  memcpy(pgm_clone->data,
         pgm_src->data,
         ( pgm_src->cols * 
           pgm_src->rows * 
           sizeof(PGMPixel)));

  return pgm_clone;
}

void pgm_delete(
  PGMData * pgm)
{
  int i;
  free(pgm->data);
  free(pgm);
}

PGMData * pgm_read(
  const char * file_name)
{
  PGMData * pgm;
  FILE    * pgm_file;
  char      version[3];
  int       rows, cols, thld;
  int       i,    j;
  int       lo,   hi;
  int       ret;

  pgm_file = fopen(file_name, "rb");
  if (pgm_file == NULL) {
    fprintf(stderr, "Could not open PGM file %s\n", file_name);
    return NULL;
  }

  char * gchar = fgets(version, sizeof(version), pgm_file);
  if (strcmp(version, "P5")) {
    fprintf(stderr, "Invalid PGM file type %s in %s, expected P5\n",
            version, file_name);
    return NULL;
  }

  pgm_file_skip_comments(pgm_file);
  ret = fscanf(pgm_file, "%d", &cols);
  pgm_file_skip_comments(pgm_file);
  ret = fscanf(pgm_file, "%d", &rows);
  pgm_file_skip_comments(pgm_file);
  ret = fscanf(pgm_file, "%d", &thld);
  pgm_file_skip_comments(pgm_file);
  ret = fgetc(pgm_file);

  wno_unused_var(ret);
  wno_unused_var(gchar);

  pgm = pgm_new(rows, cols, thld);

  if (pgm->thld > 255) {
    for (j = 0; j < pgm->cols; ++j) {
      for (i = 0; i < pgm->rows; ++i) {
        hi = fgetc(pgm_file);
        lo = fgetc(pgm_file);
        pgm->data[i * pgm->cols + j] = (PGMPixel)((hi << 8) + lo);
      }
    }
  } else {
    for (j = 0; j < pgm->cols; ++j) {
      for (i = 0; i < pgm->rows; ++i) {
        pgm->data[i * pgm->cols + j] = (PGMPixel)fgetc(pgm_file);
      }
    }
  }

  fclose(pgm_file);
  return pgm;
}

int pgm_write(
  const PGMData * pgm,
  const char    * file_name)
{
  FILE * pgm_file;
  int    i,  j;
  int    hi, lo;

  pgm_file = fopen(file_name, "wb");
  if (pgm_file == NULL) {
    fprintf(stderr, "Cannot open PGM file for writing");
    return -1;
  }

  fprintf(pgm_file, "P5 ");
  fprintf(pgm_file, "%d %d ", pgm->cols, pgm->rows);
  fprintf(pgm_file, "%d ",    pgm->thld);

  if (pgm->thld > 255) {
    for (j = 0; j < pgm->cols; j++) {
      for (i = 0; i < pgm->rows; i++) {
        int pxval = nearbyint(pgm->data[i * pgm->cols + j]);
        hi = HI(pxval);
        lo = LO(pxval);
        fputc(hi, pgm_file);
        fputc(lo, pgm_file);
      }
    }
  } else {
    for (j = 0; j < pgm->cols; j++) {
      for (i = 0; i < pgm->rows; i++) {
        int pxval = nearbyint(pgm->data[i * pgm->cols + j]);
        lo = LO(pxval);
        fputc(lo, pgm_file);
      }
    }
  }

  fclose(pgm_file);
  return 0;
}

static void pgm_file_skip_comments(
  FILE * fp)
{
  int ch, ret;

  /* Skip white spaces: */
  ret = fscanf(fp, " ");

  /* Skip lines starting with '#': */
  while ((ch = fgetc(fp)) == '#') {
    while ((ch = fgetc(fp)) != '\n'  &&  ch != EOF);
    ret = fscanf(fp," ");
  }

  /* Replace last character read: */
  ungetc(ch, fp);

  wno_unused_var(ret);
}


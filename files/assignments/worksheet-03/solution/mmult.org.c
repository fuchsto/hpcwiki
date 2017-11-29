
void mmult(
  const double A[SIZE][SIZE],
  const double B[SIZE][SIZE],
  double C[SIZE][SIZE]
) {
  int    i, j, k;
  double sum;

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      sum = C[i][j];
      for (k = 0; k < SIZE; k++) {
        sum += A[i][k] * B[k][j];
      }
      C[i][j] = sum;
    }
  }
}


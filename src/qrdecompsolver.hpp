#include "matrix.hpp"

void qrDecomposition(const Matrix &A, Matrix &Q, Matrix &R) {
  int m = A.rows();
  int n = A.cols();
  if ( (Q.cols() != m) ||  (Q.rows() != n) )
    Q = Matrix(n, m);
  if ( (R.cols() != n) ||  (R.rows() != n) )
    R = Matrix(n, n);

  Matrix v(m, 1);
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < m; i++) {
      v.matrix[i][0] = A.matrix[i][k];
    }
    for (int j = 0; j < k; j++) {
      double r = 0.0;
      for (int i = 0; i < m; i++) {
        r += Q.matrix[i][j] * v.matrix[i][0];
      }
      for (int i = 0; i < m; i++) {
        v.matrix[i][0] -= r * Q.matrix[i][j];
      }
    }
    double norm = 0.0;
    for (int i = 0; i < m; i++) {
      norm += v.matrix[i][0] * v.matrix[i][0];
    }
    norm = sqrt(norm);
    for (int i = 0; i < m; i++) {
      Q.matrix[i][k] = v.matrix[i][0] / norm;
    }
    R.matrix[k][k] = norm;
    for (int j = k + 1; j < n; j++) {
      double r = 0.0;
      for (int i = 0; i < m; i++) {
        r += Q.matrix[i][k] * A.matrix[i][j];
      }
      R.matrix[k][j] = r;
    }
  }
}

void svd(const Matrix &A, Matrix &U, Matrix &S, Matrix &Vt) {
  int m = A.rows();
  int n = A.cols();
  Matrix At = transpose(A);
  Matrix AtA;
  multiplyMatrices(At, A, AtA);
  Matrix Q, R;
  qrDecomposition(AtA, Q, R);
  Matrix eigenValues(n, 1);
  for (int i = 0; i < n; i++) {
    eigenValues.matrix[i][0] = R.matrix[i][i];
  }
  Matrix eigenVectors;
  eigenVectors = Q;
  Matrix Ut, V;
  Ut = transpose(eigenVectors);
  V = eigenVectors;
  Matrix SigmaInv;
  SigmaInv = Matrix(n, n);
  for (int i = 0; i < n; i++) {
    SigmaInv.matrix[i][i] = 1.0 / sqrt(eigenValues.matrix[i][0]);
  }
  Matrix tmp;
  multiplyMatrices(V, SigmaInv, tmp);
  multiplyMatrices(A, tmp, U);
  multiplyMatrices(SigmaInv, eigenValues, S);
  Vt = transpose(V);
}

void qrDecompositionSVD(const Matrix &A, Matrix &Q, Matrix &R) {
  Matrix U, S, Vt;
  svd(A, U, S, Vt);
  Q = U;
  multiplyMatrices(transpose(Vt), S, R);
}

void solveLinearQR(const Matrix &Q, const Matrix &R, const Matrix &b, Matrix &x, int n) {
  Matrix qt;
  qt = transpose(Q);
  Matrix Qtb;
  multiplyMatrices(qt, b, Qtb);
  for (int i = n - 1; i >= 0; i--) {
    x.matrix[i][0] = Qtb.matrix[i][0];
    for (int j = i + 1; j < n; j++) {
      x.matrix[i][0] -= R.matrix[i][j] * x.matrix[j][0];
    }
    x.matrix[i][0] /= R.matrix[i][i];
  }
}
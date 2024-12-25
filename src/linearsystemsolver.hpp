#ifndef LINEARSYSTEMSOLVER_H
#define LINEARSYSTEMSOLVER_H
#include "matrix.hpp"
#include "qrdecompsolver.hpp"

void solveLinearSystemQR(Matrix &A, Matrix &b, Matrix &x) {
    Matrix Q, R;
    qrDecomposition(A, Q, R);
    solveLinearQR(Q, R, b, x, A.rows());
}

#endif
#include "matrix.h"

Matrix::Matrix(int initRows, int initColumns)
    : std::vector<std::vector<double>>(initRows, std::vector<double>(initColumns, 0.0)), //int to first vector, then vector object to second, thus building a matrix.
    rows(initRows),
    columns(initColumns)
{}

Matrix Matrix::operator * (Matrix m){

    if (this->columns != m.rows) {
        return m;
    }
    Matrix res(this->rows, m.columns);

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < m.columns; j++) {
            res[i][j] = 0;

            for (int k = 0; k < m.rows; k++) {
                res[i][j] += (*this)[i][k] * m[k][j];
            }
        }
    }

    return res;
}

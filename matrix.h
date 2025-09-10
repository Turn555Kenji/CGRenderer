#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix : public std::vector<std::vector<double>>
{
public:
    Matrix(int initRows, int initColumns);
    Matrix operator * (Matrix m);

    //Getters for easier implementation
    int getRows() const {return rows; }
    int getColumns() const {return columns; }

private:
    int rows;
    int columns;
};

#endif // MATRIX_H

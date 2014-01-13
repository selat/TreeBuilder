#include "TreeLibMatrix3.h"

TreeLibMatrix3::TreeLibMatrix3()
{
    matrix[0][0] = 0.0;
    matrix[0][1] = 0.0;
    matrix[0][2] = 0.0;
    matrix[1][0] = 0.0;
    matrix[1][1] = 0.0;
    matrix[1][2] = 0.0;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = 0.0;
}

TreeLibMatrix3::TreeLibMatrix3(double e00, double e01, double e02,
                            double e10, double e11, double e12,
                            double e20, double e21, double e22)
{
    matrix[0][0] = e00;
    matrix[0][1] = e01;
    matrix[0][2] = e02;
    matrix[1][0] = e10;
    matrix[1][1] = e11;
    matrix[1][2] = e12;
    matrix[2][0] = e20;
    matrix[2][1] = e21;
    matrix[2][2] = e22;
}

double* TreeLibMatrix3::operator[](int index)
{
    return matrix[index];
}

TreeLibMatrix3& TreeLibMatrix3::operator*(TreeLibMatrix3& mtr)
{
    static TreeLibMatrix3 tmp_matrix;
    tmp_matrix[0][0] = matrix[0][0] * mtr[0][0] + matrix[0][1] * mtr[1][0] + matrix[0][2] * mtr[2][0];
    tmp_matrix[0][1] = matrix[0][0] * mtr[0][1] + matrix[0][1] * mtr[1][1] + matrix[0][2] * mtr[2][1];
    tmp_matrix[0][2] = matrix[0][0] * mtr[0][2] + matrix[0][1] * mtr[1][2] + matrix[0][2] * mtr[2][2];
    tmp_matrix[1][0] = matrix[1][0] * mtr[0][0] + matrix[1][1] * mtr[1][0] + matrix[1][2] * mtr[2][0];
    tmp_matrix[1][1] = matrix[1][0] * mtr[0][1] + matrix[1][1] * mtr[1][1] + matrix[1][2] * mtr[2][1];
    tmp_matrix[1][2] = matrix[1][0] * mtr[0][2] + matrix[1][1] * mtr[1][2] + matrix[1][2] * mtr[2][2];
    tmp_matrix[2][0] = matrix[2][0] * mtr[0][0] + matrix[2][1] * mtr[1][0] + matrix[2][2] * mtr[2][0];
    tmp_matrix[2][1] = matrix[2][0] * mtr[0][1] + matrix[2][1] * mtr[1][1] + matrix[2][2] * mtr[2][1];
    tmp_matrix[2][2] = matrix[2][0] * mtr[0][2] + matrix[2][1] * mtr[1][2] + matrix[2][2] * mtr[2][2];
    return tmp_matrix;
}

TreeLibMatrix3& TreeLibMatrix3::transpose()
{
    double tmp = matrix[0][1];
    matrix[0][1] = matrix[1][0];
    matrix[1][0] = tmp;

    tmp = matrix[0][2];
    matrix[0][2] = matrix[2][0];
    matrix[2][0] = tmp;

    tmp = matrix[1][2];
    matrix[1][2] = matrix[2][1];
    matrix[2][1] = tmp;
    return *this;
}

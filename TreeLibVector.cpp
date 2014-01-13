#include "TreeLibVector.h"
#include "TreeLibMatrix3.h"
#include <iostream>

TreeLibVector::TreeLibVector()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

double& TreeLibVector::operator[](int index)
{
    return *(&x + index);
}

TreeLibVector& TreeLibVector::operator-(TreeLibVector& vector)
{
    static TreeLibVector tmp_vector;
    tmp_vector.x = x - vector.x;
    tmp_vector.y = y - vector.y;
    tmp_vector.z = z - vector.z;
    return tmp_vector;
}

TreeLibVector& TreeLibVector::operator*(TreeLibMatrix3& matrix)
{
    static TreeLibVector tmp_vector;
    tmp_vector.x = x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2];
    tmp_vector.y = x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2];
    tmp_vector.z = x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2];
    return tmp_vector;
}

#ifndef TREELIBVECTOR_H
#define TREELIBVECTOR_H

#include "TreeLibMatrix3.h"

class TreeLibVector
{
public:
    TreeLibVector();
    double& operator[](int index);
    TreeLibVector& operator-(TreeLibVector& vector);
    TreeLibVector& operator*(TreeLibMatrix3& matrix);
    double x;
    double y;
    double z;
};

#endif
#ifndef TREELIBMATRIX3_H
#define TREELIBMATRIX3_H

class TreeLibMatrix3
{
public:
    TreeLibMatrix3();
    TreeLibMatrix3(double e00, double e01, double e02,
                   double e10, double e11, double e12,
                   double e20, double e21, double e22);
    double* operator[](int index);
    TreeLibMatrix3& operator*(TreeLibMatrix3& mtr);
    TreeLibMatrix3& transpose();
private:
    double matrix[3][3];
};

#endif
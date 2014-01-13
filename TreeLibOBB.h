#ifndef TREELIBOBB_H
#define TREELIBOBB_H

#include "TreeLibVector.h"
#include "TreeLibMatrix3.h"

class TreeLibOBB
{
public:
    TreeLibOBB(TreeLibVector& position, TreeLibVector& size, TreeLibMatrix3& matrix);
    void draw(double tx, double ty, double tz);
    bool overlapsOBB(TreeLibOBB& obb);
private:
    TreeLibVector position, size;
    TreeLibMatrix3 matrix;
};

#endif
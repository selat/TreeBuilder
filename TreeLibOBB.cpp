#include <cmath>
#include <iostream>
#include <windows.h>
#include <gl/gl.h>

using namespace std;

#include "TreeLibOBB.h"
#include "TreeLibVector.h"
#include "TreeLibMatrix3.h"

TreeLibOBB::TreeLibOBB(TreeLibVector& _position, TreeLibVector& _size, TreeLibMatrix3& _matrix)
{
    position = _position;
    size = _size;
    matrix = _matrix;
}

void TreeLibOBB::draw(double tx, double ty, double tz)
{
    glPushMatrix();
    static double matr[16];

    matr[0] = matrix[0][0];
    matr[1] = matrix[0][1];
    matr[2] = matrix[0][2];
    matr[3] = 0;

    matr[4] = matrix[1][0];
    matr[5] = matrix[1][1];
    matr[6] = matrix[1][2];
    matr[7] = 0;

    matr[8] = matrix[2][0];
    matr[9] = matrix[2][1];
    matr[10] = matrix[2][2];
    matr[11] = 0;

    matr[12] = position.x;
    matr[13] = position.y;
    matr[14] = position.z;
    matr[15] = 1;

    glMultMatrixd(matr);

    glBegin(GL_LINES);
        glVertex3f(position.x + size.x, position.y + size.y, position.z + size.z);
        glVertex3f(position.x - size.x, position.y + size.y, position.z + size.z);
        glVertex3f(position.x + size.x, position.y + size.y, position.z + size.z);
        glVertex3f(position.x + size.x, position.y - size.y, position.z + size.z);
        glVertex3f(position.x + size.x, position.y + size.y, position.z + size.z);
        glVertex3f(position.x + size.x, position.y + size.y, position.z - size.z);

        glVertex3f(position.x - size.x, position.y - size.y, position.z - size.z);
        glVertex3f(position.x + size.x, position.y - size.y, position.z - size.z);
        glVertex3f(position.x - size.x, position.y - size.y, position.z - size.z);
        glVertex3f(position.x - size.x, position.y + size.y, position.z - size.z);
        glVertex3f(position.x - size.x, position.y - size.y, position.z - size.z);
        glVertex3f(position.x - size.x, position.y - size.y, position.z + size.z);

        glVertex3f(position.x + size.x, position.y - size.y, position.z + size.z);
        glVertex3f(position.x - size.x, position.y - size.y, position.z + size.z);
        glVertex3f(position.x + size.x, position.y - size.y, position.z + size.z);
        glVertex3f(position.x + size.x, position.y - size.y, position.z - size.z);

        glVertex3f(position.x - size.x, position.y + size.y, position.z - size.z);
        glVertex3f(position.x + size.x, position.y + size.y, position.z - size.z);
        glVertex3f(position.x - size.x, position.y + size.y, position.z - size.z);
        glVertex3f(position.x - size.x, position.y + size.y, position.z + size.z);

        glVertex3f(position.x - size.x, position.y + size.y, position.z + size.z);
        glVertex3f(position.x - size.x, position.y - size.y, position.z + size.z);
        glVertex3f(position.x + size.x, position.y + size.y, position.z - size.z);
        glVertex3f(position.x + size.x, position.y - size.y, position.z - size.z);
    glEnd();

    glPopMatrix();
}

bool TreeLibOBB::overlapsOBB(TreeLibOBB& obb)
{
    TreeLibVector a = size;
    TreeLibVector b = obb.size;
    TreeLibVector aposition = position;
    TreeLibVector bposition = obb.position;

    TreeLibMatrix3 A = matrix;
    TreeLibMatrix3 B = obb.matrix;

    TreeLibVector v = bposition - aposition;

    TreeLibVector T = v * A;

    TreeLibMatrix3 R(A * B.transpose());

    double ra, rb, t;

    for(int i = 0; i < 3; ++i)
    {
        ra = a[i];
        rb = b[0] * fabs(R[i][0]) + b[1] * fabs(R[i][1]) + b[2] * fabs(R[i][2]);
        t = fabs(T[i]);
        if(t > ra + rb)
        {
            return false;
        }
    }

    for(int i = 0; i < 3; ++i)
    {
        ra = a[0] * fabs(R[0][i]) + a[1] * fabs(R[1][i]) + a[2] * fabs(R[2][i]);
        rb = b[i];
        t = fabs(T[0] * R[0][i] + T[1] * R[1][i] + T[2] * R[2][i]);
        if(t > ra + rb)
        {
            return false;
        }
    }

    //L = A0 x B0
    ra = a[1] * fabs(R[2][0]) + a[2] * fabs(R[1][0]);
    rb = b[1] * fabs(R[0][2]) + b[2] * fabs(R[0][1]);
    t = fabs(T[2] * R[1][0] - T[1] * R[2][0]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A0 x B1
    ra = a[1] * fabs(R[2][1]) + a[2] * fabs(R[1][1]);
    rb = b[0] * fabs(R[0][2]) + b[2] * fabs(R[0][0]);
    t = fabs(T[2] * R[1][1] - T[1] * R[2][1]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A0 x B2
    ra = a[1] * fabs(R[2][2]) + a[2] * fabs(R[1][2]);
    rb = b[0] * fabs(R[0][1]) + b[1] * fabs(R[0][0]);
    t = fabs(T[2] * R[1][2] - T[1] * R[2][2]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A1 x B0
    ra = a[0] * fabs(R[2][0]) + a[2] * fabs(R[0][0]);
    rb = b[1] * fabs(R[1][2]) + b[2] * fabs(R[1][1]);
    t = fabs(T[0] * R[2][0] - T[2] * R[0][0]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A1 x B1
    ra = a[0] * fabs(R[2][1]) + a[2] * fabs(R[0][1]);
    rb = b[0] * fabs(R[1][2]) + b[2] * fabs(R[1][0]);
    t = fabs(T[0] * R[2][1] - T[2] * R[0][1]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A1 x B2
    ra = a[0] * fabs(R[2][2]) + a[2] * fabs(R[0][2]);
    rb = b[0] * fabs(R[1][1]) + b[1] * fabs(R[1][0]);
    t = fabs(T[0] * R[2][2] - T[2] * R[0][2]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A2 x B0
    ra = a[0] * fabs(R[1][0]) + a[1] * fabs(R[0][0]);
    rb = b[1] * fabs(R[2][2]) + b[2] * fabs(R[2][1]);
    t = fabs(T[1] * R[0][0] - T[0] * R[1][0]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A2 x B1
    ra = a[0] * fabs(R[1][1]) + a[1] * fabs(R[0][1]);
    rb = b[0] * fabs(R[2][2]) + b[2] * fabs(R[2][0]);
    t = fabs(T[1] * R[0][1] - T[0] * R[1][1]);
    if(t > ra + rb)
    {
        return false;
    }

    //L = A2 x B2
    ra = a[0] * fabs(R[1][2]) + a[1] * fabs(R[0][2]);
    rb = b[0] * fabs(R[2][1]) + b[1] * fabs(R[2][0]);
    t = fabs(T[1] * R[0][2] - T[0] * R[1][2]);
    if(t > ra + rb)
    {
        return false;
    }
    return true;
}

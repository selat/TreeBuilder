#ifndef POSITIONMANIPULATOR_H
#define POSITIONMANIPULATOR_H

class QPoint;

struct LengthManipulator
{
    QPoint position;
    float angle;
    float currentLength;
    float maxLength;
};

struct PairLengthManipulator
{
    QPoint position;
    float angle;
    float currentLength1, currentLength2;
    float maxLength;
};

#endif // POSITIONMANIPULATOR_H

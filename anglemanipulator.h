#ifndef ANGLEMANIPULATOR_H
#define ANGLEMANIPULATOR_H

class QPoint;
class DoubleSpinBox;
class QCheckBox;
class PairAngleEditWidget;

struct AngleManipulator
{
    QPoint position;
    float radius;
    float angle;
    float minAngle, maxAngle;

    DoubleSpinBox *angleEdit;
};

struct PairAngleManipulator
{
    QPoint position;
    float radius;
    float angle1, angle2;
    float minAngle, maxAngle;

    PairAngleEditWidget *angleEdit;
};

#endif // ANGLEMANIPULATOR_H

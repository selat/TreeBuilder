#include "doublespinbox.h"

#include <QtWidgets>

DoubleSpinBox::DoubleSpinBox(Type::Manipulator _manipulatorType, int _id, int _currentPoint, QWidget *parent) :
    QDoubleSpinBox(parent)
{
    manipulatorType = _manipulatorType;
    id = _id;
    currentPoint = _currentPoint;
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
}

void DoubleSpinBox::changeValue(double value)
{
    emit valueChanged(manipulatorType, id, currentPoint, value);
}

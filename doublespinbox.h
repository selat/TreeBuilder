#ifndef DOUBLESPINBOX_H
#define DOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QWidget>

#include "visualeditwidgettypes.h"

class DoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit DoubleSpinBox(Type::Manipulator _manipulatorType, int _id, int _currentPoint, QWidget *parent = 0);
    
signals:
    void valueChanged(Type::Manipulator manipulatorType, int id, int currentPoint, double value);

public slots:
    void changeValue(double value);

private:
    Type::Manipulator manipulatorType;
    int id;
    int currentPoint;
};

#endif // DOUBLESPINBOX_H

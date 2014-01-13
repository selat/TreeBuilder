#ifndef PAIRANGLEEDITWIDGET_H
#define PAIRANGLEEDITWIDGET_H

#include <QtWidgets>

#include "visualeditwidgettypes.h"

class PairAngleEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PairAngleEditWidget(int _id, float _angle1, float _angle2, float _minAngle, float _maxAngle, QWidget *parent = 0);
    void setFirstAngle(double value);
    void setSecondAngle(double value);

signals:
    void angleValueChanged(Type::Manipulator manipulatorType, int id, int currentPoint, double value);

public slots:
    void firstAngleChanged(double value);
    void secondAngleChanged(double value);

private:
    void createWidgets();
    void createConnections();

    int id;
    float angle1;
    float angle2;
    float minAngle;
    float maxAngle;

    QDoubleSpinBox *firstAngleEdit;
    QDoubleSpinBox *secondAngleEdit;
    QCheckBox *lockMove;
    
};

#endif // PAIRANGLEEDITWIDGET_H

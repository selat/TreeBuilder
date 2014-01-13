#include "pairangleeditwidget.h"

PairAngleEditWidget::PairAngleEditWidget(int _id, float _angle1, float _angle2, float _minAngle, float _maxAngle, QWidget *parent) :
    QWidget(parent)
{
    id = _id;
    angle1 = _angle1;
    angle2 = _angle2;
    minAngle = _minAngle;
    maxAngle = _maxAngle;
    setFixedSize(220, 60);
    createWidgets();
    createConnections();
}

void PairAngleEditWidget::setFirstAngle(double value)
{
    firstAngleEdit -> setValue(value);
}

void PairAngleEditWidget::setSecondAngle(double value)
{
    secondAngleEdit -> setValue(value);
}

void PairAngleEditWidget::firstAngleChanged(double value)
{
    if(value > secondAngleEdit -> value())
    {
        firstAngleEdit -> setValue(secondAngleEdit -> value());
    } else {
        emit angleValueChanged(Type::PairAngleManipulator, id, 1, value);
    }
}

void PairAngleEditWidget::secondAngleChanged(double value)
{
    if(value < firstAngleEdit -> value())
    {
        secondAngleEdit -> setValue(firstAngleEdit -> value());
    } else {
        emit angleValueChanged(Type::PairAngleManipulator, id, 2, value);
    }
}

void PairAngleEditWidget::createWidgets()
{
    double maximum;
    double angleEdit1, angleEdit2;
    if(minAngle <= maxAngle)
    {
        maximum = (maxAngle - minAngle) / M_PI * 180.0;
        angleEdit1 = (angle1 - minAngle) / M_PI * 180.0;
        angleEdit2 = (angle2 - minAngle) / M_PI * 180.0;
    } else {
        maximum = (M_PI * 2.0 - minAngle + maxAngle) / M_PI * 180.0;
        if(angle1 > minAngle)
        {
            angleEdit1 = (angle1 - minAngle) / M_PI * 180.0;
        } else {
            angleEdit1 = (angle1 + M_PI * 2.0 - minAngle) / M_PI * 180.0;
        }
        if(angle2 > minAngle)
        {
            angleEdit2 = (angle2 - minAngle) / M_PI * 180.0;
        } else {
            angleEdit2 = (angle2 + M_PI * 2.0 - minAngle) / M_PI * 180.0;
        }
    }

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *angleEditLayout = new QHBoxLayout();
    firstAngleEdit = new QDoubleSpinBox(this);
    firstAngleEdit -> setFixedSize(100, 20);
    firstAngleEdit -> setMinimum(0.0);
    firstAngleEdit -> setMaximum(maximum);
    firstAngleEdit -> setValue(angleEdit1);
    angleEditLayout -> addWidget(firstAngleEdit);

    secondAngleEdit = new QDoubleSpinBox(this);
    secondAngleEdit -> setFixedSize(100, 20);
    secondAngleEdit -> setMinimum(0.0);
    secondAngleEdit -> setMaximum(maximum);
    secondAngleEdit -> setValue(angleEdit2);
    angleEditLayout -> addWidget(secondAngleEdit);

    layout -> addLayout(angleEditLayout);

    QHBoxLayout *lockMoveLayout = new QHBoxLayout();
    lockMove = new QCheckBox(this);
    lockMoveLayout -> addWidget(lockMove);

    QLabel *lockMoveLabel = new QLabel(tr("Lock move"), this);
    lockMoveLayout -> addWidget(lockMoveLabel);

    layout -> addLayout(lockMoveLayout);

    setLayout(layout);
}

void PairAngleEditWidget::createConnections()
{
    connect(firstAngleEdit, SIGNAL(valueChanged(double)), this, SLOT(firstAngleChanged(double)));
    connect(secondAngleEdit, SIGNAL(valueChanged(double)), this, SLOT(secondAngleChanged(double)));
}

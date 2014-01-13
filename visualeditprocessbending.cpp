#include "visualeditprocessbending.h"

VisualEditProcessBending::VisualEditProcessBending(QWidget *parent) :
    VisualEditWidget(parent)
{
    setQuadWidth(16);
    trunkBendingEdit = addPairAngleManipulator(QPoint(320, 260), 160.0f, M_PI_2 - 0.5, M_PI_2 - 0.3, M_PI_2 * 3.0, M_PI_2);
}

double VisualEditProcessBending::getMinAngle()
{
    return getPairAngleManipulator(trunkBendingEdit) -> angle1;
}

double VisualEditProcessBending::getMaxAngle()
{
    return getPairAngleManipulator(trunkBendingEdit) -> angle2;
}

void VisualEditProcessBending::setMinAngle(double value)
{
    getPairAngleManipulator(trunkBendingEdit) -> angle1 = value;
}

void VisualEditProcessBending::setMaxAngle(double value)
{
    getPairAngleManipulator(trunkBendingEdit) -> angle2 = value;
}

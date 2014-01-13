#include "visualeditwidget.h"
#include "doublespinbox.h"
#include "pairangleeditwidget.h"

VisualEditWidget::VisualEditWidget(QWidget *parent) :
    QDialog(parent), e(0.0001f)
{
    dragging = false;
    quadWidth = 10;
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(640, 480);
    createWidgets();
    createConnections();
}

void VisualEditWidget::setQuadWidth(int value)
{
    quadWidth = value;
}

int VisualEditWidget::addLengthManipulator(QPoint position, float angle, float currentLength, float maxLength)
{
    LengthManipulator tmp;
    tmp.position = position;
    tmp.angle = angle;
    tmp.currentLength = currentLength;
    tmp.maxLength = maxLength;
    lengthManipulators.push_back(tmp);
    return (lengthManipulators.size() - 1);
}

int VisualEditWidget::addAngleManipulator(QPoint position, float radius, float angle, float minAngle, float maxAngle)
{
    AngleManipulator tmp;
    tmp.position = position;
    tmp.radius = radius;
    tmp.angle = angle;
    tmp.minAngle = minAngle;
    tmp.maxAngle = maxAngle;

    int minY;
    if(minAngle <= maxAngle)
    {
        if(maxAngle <= M_PI)
        {
            minY = position.y();
        } else if(maxAngle <= M_PI_2 * 3.0f)
        {
            minY = position.y() + qSin(maxAngle) * radius;
        } else if(minAngle <= M_PI_2 * 3.0f)
        {
            minY = position.y() - radius;
        } else if(qSin(minAngle) <= qSin(maxAngle))
        {
            minY = position.y() + qSin(minAngle) * radius;
        } else {
            minY = position.y() + qSin(maxAngle) * radius;
        }
    } else {
        if(minAngle <= M_PI_2 * 3.0f)
        {
            minY = position.y() - radius;
        } else if(maxAngle >= M_PI_2 * 3.0f)
        {
            minY = position.y() - radius;
        } else if(qSin(minAngle) <= qSin(maxAngle))
        {
            minY = position.y() + qSin(minAngle) * radius;
        } else {
            minY = position.y() + qSin(maxAngle) * radius;
        }
    }

    tmp.angleEdit = new DoubleSpinBox(Type::AngleManipulator, angleManipulators.size(), 0, this);
    tmp.angleEdit -> setFixedSize(100, 20);
    tmp.angleEdit -> move(position.x() - tmp.angleEdit -> width() / 2,
                          height() - minY + 10);
    tmp.angleEdit -> setMinimum(0.0f);

    // Value between zero and max delta angle
    if(minAngle <= maxAngle)
    {
        tmp.angleEdit -> setMaximum((maxAngle - minAngle) / M_PI * 180.0);
    } else {
        tmp.angleEdit -> setMaximum((M_PI * 2.0 - minAngle + maxAngle) / M_PI * 180.0);
    }
    double angleEditValue = (minAngle + angle) / M_PI * 180.0f;
    if(angleEditValue >= 360.0f)
    {
        angleEditValue -= 360.0f;
    }
    tmp.angleEdit -> setValue(angleEditValue);

    connect(tmp.angleEdit, SIGNAL(valueChanged(Type::Manipulator, int, int, double)),
                                  this, SLOT(manualValueEdit(Type::Manipulator, int, int, double)));

    angleManipulators.push_back(tmp);
    return (angleManipulators.size() - 1);
}

int VisualEditWidget::addPairLengthManipulator(QPoint position, float angle, float currentLength1,
                                                float currentLength2, float maxLength)
{
    PairLengthManipulator tmp;
    tmp.position = position;
    tmp.angle = angle;
    tmp.currentLength1 = currentLength1;
    tmp.currentLength2 = currentLength2;
    tmp.maxLength = maxLength;
    pairLengthManipulators.push_back(tmp);
    return (pairLengthManipulators.size() - 1);
}

int VisualEditWidget::addPairAngleManipulator(QPoint position, float radius, float angle1, float angle2,
                                               float minAngle, float maxAngle)
{
    PairAngleManipulator tmp;
    tmp.position = position;
    tmp.radius = radius;
    tmp.angle1 = angle1;
    tmp.angle2 = angle2;
    tmp.minAngle = minAngle;
    tmp.maxAngle = maxAngle;

    int minY;
    if(minAngle <= maxAngle)
    {
        if(maxAngle <= M_PI)
        {
            minY = position.y();
        } else if(maxAngle <= M_PI_2 * 3.0f)
        {
            minY = position.y() + qSin(maxAngle) * radius;
        } else if(minAngle <= M_PI_2 * 3.0f)
        {
            minY = position.y() - radius;
        } else if(qSin(minAngle) <= qSin(maxAngle))
        {
            minY = position.y() + qSin(minAngle) * radius;
        } else {
            minY = position.y() + qSin(maxAngle) * radius;
        }
    } else {
        if(minAngle <= M_PI_2 * 3.0f)
        {
            minY = position.y() - radius;
        } else if(maxAngle >= M_PI_2 * 3.0f)
        {
            minY = position.y() - radius;
        } else if(qSin(minAngle) <= qSin(maxAngle))
        {
            minY = position.y() + qSin(minAngle) * radius;
        } else {
            minY = position.y() + qSin(maxAngle) * radius;
        }
    }

    tmp.angleEdit = new PairAngleEditWidget(pairAngleManipulators.size(), angle1, angle2, minAngle, maxAngle, this);
    tmp.angleEdit -> move(position.x() - tmp.angleEdit -> width() / 2,
                          height() - minY + 10);
    connect(tmp.angleEdit, SIGNAL(angleValueChanged(Type::Manipulator, int, int, double)),
            this, SLOT(manualValueEdit(Type::Manipulator, int, int, double)));
    pairAngleManipulators.push_back(tmp);
    return (pairAngleManipulators.size() - 1);
}

LengthManipulator* VisualEditWidget::getLengthManipulator(int id)
{
    return &lengthManipulators[id];
}

AngleManipulator* VisualEditWidget::getAngleManipulator(int id)
{
    return &angleManipulators[id];
}

PairLengthManipulator* VisualEditWidget::getPairLengthManipulator(int id)
{
    return &pairLengthManipulators[id];
}

PairAngleManipulator* VisualEditWidget::getPairAngleManipulator(int id)
{
    return &pairAngleManipulators[id];
}

void VisualEditWidget::mousePressEvent(QMouseEvent *event)
{
    if(event -> button() == Qt::LeftButton)
    {
        int x1, y1;
        int mouseX = event -> x();
        int mouseY = height() - event -> y();
        for(int i = 0; i < lengthManipulators.size(); ++i)
        {
            x1 = lengthManipulators[i].position.x() + lengthManipulators[i].currentLength * qCos(lengthManipulators[i].angle);
            y1 = lengthManipulators[i].position.y() + lengthManipulators[i].currentLength * qSin(lengthManipulators[i].angle);
            if(((mouseX >= x1 - quadWidth / 2) && (mouseX <= x1 + quadWidth / 2))
                    && ((mouseY >= y1 - quadWidth / 2) && (mouseY <= y1 + quadWidth / 2)))
            {
                draggingID = i;
                draggingType = Type::LengthManipulator;
                dragging = true;
                break;
            }
        }

        if(!dragging)
        {
            for(int i = 0; i < angleManipulators.size(); ++i)
            {
                x1 = angleManipulators[i].position.x() + angleManipulators[i].radius * qCos(angleManipulators[i].angle);
                y1 = angleManipulators[i].position.y() + angleManipulators[i].radius * qSin(angleManipulators[i].angle);
                if(((mouseX >= x1 - quadWidth / 2) && (mouseX <= x1 + quadWidth / 2))
                        && ((mouseY >= y1 - quadWidth / 2) && (mouseY <= y1 + quadWidth / 2)))
                {
                    draggingID = i;
                    draggingType = Type::AngleManipulator;
                    dragging = true;
                    break;
                }
            }
        }

        if(!dragging)
        {
            for(int i = 0; i < pairLengthManipulators.size(); ++i)
            {
                x1 = pairLengthManipulators[i].position.x() + pairLengthManipulators[i].currentLength1 * qCos(pairLengthManipulators[i].angle);
                y1 = pairLengthManipulators[i].position.y() + pairLengthManipulators[i].currentLength1 * qSin(pairLengthManipulators[i].angle);
                if(((mouseX >= x1 - quadWidth / 2) && (mouseX <= x1 + quadWidth / 2))
                        && ((mouseY >= y1 - quadWidth / 2) && (mouseY <= y1 + quadWidth / 2)))
                {
                    draggingID = i;
                    draggingPoint = 1;
                    draggingType = Type::PairLengthManipulator;
                    dragging = true;
                    break;
                }

                x1 = pairLengthManipulators[i].position.x() + pairLengthManipulators[i].currentLength2 * qCos(pairLengthManipulators[i].angle);
                y1 = pairLengthManipulators[i].position.y() + pairLengthManipulators[i].currentLength2 * qSin(pairLengthManipulators[i].angle);
                if(((mouseX >= x1 - quadWidth / 2) && (mouseX <= x1 + quadWidth / 2))
                        && ((mouseY >= y1 - quadWidth / 2) && (mouseY <= y1 + quadWidth / 2)))
                {
                    draggingID = i;
                    draggingPoint = 2;
                    draggingType = Type::PairLengthManipulator;
                    dragging = true;
                    break;
                }
            }
        }

        if(!dragging)
        {
            for(int i = 0; i < pairAngleManipulators.size(); ++i)
            {
                x1 = pairAngleManipulators[i].position.x() + pairAngleManipulators[i].radius * qCos(pairAngleManipulators[i].angle1);
                y1 = pairAngleManipulators[i].position.y() + pairAngleManipulators[i].radius * qSin(pairAngleManipulators[i].angle1);
                if(((mouseX >= x1 - quadWidth / 2) && (mouseX <= x1 + quadWidth / 2))
                        && ((mouseY >= y1 - quadWidth / 2) && (mouseY <= y1 + quadWidth / 2)))
                {
                    draggingID = i;
                    draggingPoint = 1;
                    draggingType = Type::PairAngleManipulator;
                    dragging = true;
                    break;
                }

                x1 = pairAngleManipulators[i].position.x() + pairAngleManipulators[i].radius * qCos(pairAngleManipulators[i].angle2);
                y1 = pairAngleManipulators[i].position.y() + pairAngleManipulators[i].radius * qSin(pairAngleManipulators[i].angle2);
                if(((mouseX >= x1 - quadWidth / 2) && (mouseX <= x1 + quadWidth / 2))
                        && ((mouseY >= y1 - quadWidth / 2) && (mouseY <= y1 + quadWidth / 2)))
                {
                    draggingID = i;
                    draggingPoint = 2;
                    draggingType = Type::PairAngleManipulator;
                    dragging = true;
                    break;
                }
            }
        }
    }
    repaint();
}

void VisualEditWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(dragging)
    {
        int mouseX = event -> x();
        int mouseY = height() - event -> y();
        int x1, y1;
        int x2, y2;
        switch(draggingType)
        {
        case Type::LengthManipulator:
        {
            x1 = lengthManipulators[draggingID].position.x();
            y1 = lengthManipulators[draggingID].position.y();
            x2 = x1 + lengthManipulators[draggingID].maxLength * qCos(lengthManipulators[draggingID].angle);
            y2 = y1 + lengthManipulators[draggingID].maxLength * qSin(lengthManipulators[draggingID].angle);
            if(qAbs(x2 - x1) >= qAbs(y2 - y1))
            {
                if((mouseX >= x1) && (mouseX <= x2))
                {
                    lengthManipulators[draggingID].currentLength = (mouseX - x1) / qCos(lengthManipulators[draggingID].angle);
                }
            } else {
                if((mouseY >= y1) && (mouseY <= y2))
                {
                    lengthManipulators[draggingID].currentLength = (mouseY - y1) / qSin(lengthManipulators[draggingID].angle);
                } else if((mouseY >= y2) && (mouseY <= y1))
                {
                    lengthManipulators[draggingID].currentLength = (y1 - mouseY) / qSin(lengthManipulators[draggingID].angle);
                }
            }
            break;
        }
        case Type::AngleManipulator:
        {
            x1 = angleManipulators[draggingID].position.x();
            y1 = angleManipulators[draggingID].position.y();
            float angle = qAtan2(mouseY - y1, mouseX - x1);
            if(angle < 0.0f)
            {
                angle += M_PI * 2.0f;
            }
            if(angleManipulators[draggingID].minAngle <= angleManipulators[draggingID].maxAngle)
            {
                if((angle >= angleManipulators[draggingID].minAngle) && (angle <= angleManipulators[draggingID].maxAngle))
                {
                    angleManipulators[draggingID].angle = angle;
                    double angleEditValue = (angle - angleManipulators[draggingID].minAngle) / M_PI * 180.0;
                    angleManipulators[draggingID].angleEdit -> setValue(angleEditValue);
                }
            } else {
                if((angle >= angleManipulators[draggingID].minAngle) || (angle <= angleManipulators[draggingID].maxAngle))
                {
                    angleManipulators[draggingID].angle = angle;
                    double angleEditValue;
                    if(angle >= angleManipulators[draggingID].minAngle)
                    {
                        angleEditValue = angle - angleManipulators[draggingID].minAngle;
                    } else {
                        angleEditValue = M_PI * 2.0 - angleManipulators[draggingID].minAngle + angle;
                    }
                    angleEditValue *= 180.0 / M_PI;

                    angleManipulators[draggingID].angleEdit -> setValue(angleEditValue);
                }
            }
            break;
        }
        case Type::PairLengthManipulator:
        {
            x1 = pairLengthManipulators[draggingID].position.x();
            y1 = pairLengthManipulators[draggingID].position.y();
            x2 = x1 + pairLengthManipulators[draggingID].maxLength * qCos(pairLengthManipulators[draggingID].angle);
            y2 = y1 + pairLengthManipulators[draggingID].maxLength * qSin(pairLengthManipulators[draggingID].angle);
            if(qAbs(x2 - x1) >= qAbs(y2 - y1))
            {
                if((mouseX >= x1) && (mouseX <= x2))
                {
                    if((draggingPoint == 1) &&
                        ((mouseX - x1) / qCos(pairLengthManipulators[draggingID].angle) <= pairLengthManipulators[draggingID].currentLength2))
                    {
                        pairLengthManipulators[draggingID].currentLength1 = (mouseX - x1) / qCos(pairLengthManipulators[draggingID].angle);
                    } else if((draggingPoint == 2) &&
                        ((mouseX - x1) / qCos(pairLengthManipulators[draggingID].angle) >= pairLengthManipulators[draggingID].currentLength1))
                    {
                        pairLengthManipulators[draggingID].currentLength2 = (mouseX - x1) / qCos(pairLengthManipulators[draggingID].angle);
                    }
                }
            } else {
                if((mouseY >= y1) && (mouseY <= y2))
                {
                    if((draggingPoint == 1) &&
                        ((mouseY - y1) / qSin(pairLengthManipulators[draggingID].angle) <= pairLengthManipulators[draggingID].currentLength2))
                    {
                        pairLengthManipulators[draggingID].currentLength1 = (mouseY - y1) / qSin(pairLengthManipulators[draggingID].angle);
                    } else if((draggingPoint == 2) &&
                        ((mouseY - y1) / qSin(pairLengthManipulators[draggingID].angle) >= pairLengthManipulators[draggingID].currentLength1))
                    {
                        pairLengthManipulators[draggingID].currentLength2 = (mouseY - y1) / qSin(pairLengthManipulators[draggingID].angle);
                    }
                } else if((mouseY >= y2) && (mouseY <= y1))
                {
                    if((draggingPoint == 1) &&
                        ((mouseY - y1) / qSin(pairLengthManipulators[draggingID].angle) <= pairLengthManipulators[draggingID].currentLength2))
                    {
                        pairLengthManipulators[draggingID].currentLength1 = (y1 - mouseY) / qSin(pairLengthManipulators[draggingID].angle);
                    } else if((draggingPoint == 2) &&
                        ((mouseY - y1) / qSin(pairLengthManipulators[draggingID].angle) >= pairLengthManipulators[draggingID].currentLength1))
                    {
                        pairLengthManipulators[draggingID].currentLength2 = (y1 - mouseY) / qSin(pairLengthManipulators[draggingID].angle);
                    }
                }
            }
            break;
        }
        case Type::PairAngleManipulator:
        {
            x1 = pairAngleManipulators[draggingID].position.x();
            y1 = pairAngleManipulators[draggingID].position.y();
            float angle = qAtan2(mouseY - y1, mouseX - x1);
            if(angle < 0.0f)
            {
                angle += M_PI * 2.0f;
            }
            if(pairAngleManipulators[draggingID].minAngle <= pairAngleManipulators[draggingID].maxAngle)
            {
                if(draggingPoint == 1)
                {
                    if((angle >= pairAngleManipulators[draggingID].minAngle) && (angle <= pairAngleManipulators[draggingID].maxAngle) &&
                            (angle <= pairAngleManipulators[draggingID].angle2))
                    {
                        pairAngleManipulators[draggingID].angle1 = angle;
                        double angleEditValue = (angle - pairAngleManipulators[draggingID].minAngle) / M_PI * 180.0;
                        pairAngleManipulators[draggingID].angleEdit -> setFirstAngle(angleEditValue);
                    }
                } else {
                    if((angle >= pairAngleManipulators[draggingID].minAngle) && (angle <= pairAngleManipulators[draggingID].maxAngle) &&
                            (angle >= pairAngleManipulators[draggingID].angle1))
                    {
                        pairAngleManipulators[draggingID].angle2 = angle;
                        double angleEditValue = (angle - pairAngleManipulators[draggingID].minAngle) / M_PI * 180.0;
                        pairAngleManipulators[draggingID].angleEdit -> setSecondAngle(angleEditValue);
                    }
                }
            } else {
                // It's magic - don't touch!
                if(draggingPoint == 1)
                {
                    if(((angle >= pairAngleManipulators[draggingID].minAngle) || (angle <= pairAngleManipulators[draggingID].maxAngle)) &&
                        (((angle <= pairAngleManipulators[draggingID].angle2) && (angle <= pairAngleManipulators[draggingID].maxAngle) &&
                         (pairAngleManipulators[draggingID].angle2 <= pairAngleManipulators[draggingID].maxAngle))  ||
                          ((angle >= pairAngleManipulators[draggingID].minAngle) &&
                           (pairAngleManipulators[draggingID].angle2 <= pairAngleManipulators[draggingID].maxAngle)) ||
                            ((angle <= pairAngleManipulators[draggingID].angle2) && (angle >= pairAngleManipulators[draggingID].minAngle) &&
                             (pairAngleManipulators[draggingID].angle2 >= pairAngleManipulators[draggingID].minAngle))))
                    {
                        pairAngleManipulators[draggingID].angle1 = angle;
                        double angleEditValue;
                        if(angle >= pairAngleManipulators[draggingID].minAngle)
                        {
                            angleEditValue = angle - pairAngleManipulators[draggingID].minAngle;
                        } else {
                            angleEditValue = M_PI * 2.0 - pairAngleManipulators[draggingID].minAngle + angle;
                        }
                        angleEditValue *= 180.0 / M_PI;
                        pairAngleManipulators[draggingID].angleEdit -> setFirstAngle(angleEditValue);
                    }
                } else {
                    if(((angle >= pairAngleManipulators[draggingID].minAngle) || (angle <= pairAngleManipulators[draggingID].maxAngle)) &&
                        (((angle >= pairAngleManipulators[draggingID].angle1) && (angle <= pairAngleManipulators[draggingID].maxAngle) &&
                         (pairAngleManipulators[draggingID].angle1 <= pairAngleManipulators[draggingID].maxAngle))  ||
                          ((angle <= pairAngleManipulators[draggingID].maxAngle) &&
                           (pairAngleManipulators[draggingID].angle1 >= pairAngleManipulators[draggingID].minAngle)) ||
                            ((angle >= pairAngleManipulators[draggingID].angle1) && (angle >= pairAngleManipulators[draggingID].minAngle) &&
                             (pairAngleManipulators[draggingID].angle1 >= pairAngleManipulators[draggingID].minAngle))))
                    {
                        pairAngleManipulators[draggingID].angle2 = angle;
                        double angleEditValue;
                        if(angle >= pairAngleManipulators[draggingID].minAngle)
                        {
                            angleEditValue = angle - pairAngleManipulators[draggingID].minAngle;
                        } else {
                            angleEditValue = M_PI * 2.0 - pairAngleManipulators[draggingID].minAngle + angle;
                        }
                        angleEditValue *= 180.0 / M_PI;
                        pairAngleManipulators[draggingID].angleEdit -> setSecondAngle(angleEditValue);
                    }
                }
            }
            break;
        }
        }
    }
    repaint();
}

void VisualEditWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event -> button() == Qt::LeftButton)
    {
        if(dragging)
        {
            dragging = false;
        }
    }
    repaint();
}

void VisualEditWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(0, 0, width(), height(), Qt::white);
    painter.setTransform(QTransform(1.0, 0.0 ,0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0));
    painter.translate(0, -height());
    int x1, y1;
    int x2, y2;
    painter.setBrush(QBrush(Qt::red));

    for(int i = 0; i < lengthManipulators.size(); ++i)
    {
        if(dragging && (draggingType == Type::LengthManipulator) && (i == draggingID))
        {
            painter.setBrush(QBrush(Qt::blue));
        } else {
            painter.setBrush(QBrush(Qt::red));
        }
        x1 = lengthManipulators[i].position.x();
        y1 = lengthManipulators[i].position.y();
        x2 = x1 + lengthManipulators[i].maxLength * qCos(lengthManipulators[i].angle);
        y2 = y1 + lengthManipulators[i].maxLength * qSin(lengthManipulators[i].angle);
        painter.drawLine(x1, y1, x2, y2);

        x1 += lengthManipulators[i].currentLength * qCos(lengthManipulators[i].angle) - quadWidth / 2;
        y1 += lengthManipulators[i].currentLength * qSin(lengthManipulators[i].angle) - quadWidth / 2;
        painter.drawRect(x1, y1, quadWidth, quadWidth);
    }

    for(int i = 0; i < angleManipulators.size(); ++i)
    {
        painter.setPen(QColor(0, 0, 0, 0));
        painter.setBrush(QColor(0, 255, 0));
        if(angleManipulators[i].minAngle <= angleManipulators[i].maxAngle)
        {
            x1 = angleManipulators[i].position.x() - angleManipulators[i].radius;
            y1 = angleManipulators[i].position.y() - angleManipulators[i].radius;

            int startAngle = -angleManipulators[i].minAngle / M_PI * 180.0f * 16.0f;
            int spanAngle = (angleManipulators[i].minAngle - angleManipulators[i].maxAngle) / M_PI * 180.0f * 16.0f;

            painter.drawPie(x1, y1, angleManipulators[i].radius * 2.0f, angleManipulators[i].radius * 2.0f, startAngle, spanAngle);
        } else {
            x1 = angleManipulators[i].position.x() - angleManipulators[i].radius;
            y1 = angleManipulators[i].position.y() - angleManipulators[i].radius;

            int startAngle = -angleManipulators[i].minAngle / M_PI * 180.0f * 16.0f;
            int spanAngle = -(angleManipulators[i].maxAngle + M_PI * 2.0 - angleManipulators[i].minAngle) / M_PI * 180.0f * 16.0f;

            painter.drawPie(x1, y1, angleManipulators[i].radius * 2.0f, angleManipulators[i].radius * 2.0f, startAngle, spanAngle);
        }
        painter.fillRect(x1, y1, angleManipulators[i].radius * 2.0f, angleManipulators[i].radius * 2.0f, QBrush(QColor(255, 255, 255, 192)));
        painter.setPen(QColor(0, 0, 0));

        if(dragging && (draggingType == Type::AngleManipulator) && (i == draggingID))
        {
            painter.setBrush(QBrush(Qt::blue));
        } else {
            painter.setBrush(QBrush(Qt::red));
        }

        x1 = angleManipulators[i].position.x();
        y1 = angleManipulators[i].position.y();

        x2 = x1 + angleManipulators[i].radius * qCos(angleManipulators[i].angle);
        y2 = y1 + angleManipulators[i].radius * qSin(angleManipulators[i].angle);

        painter.drawLine(x1, y1, x2, y2);

        x2 -= quadWidth / 2;
        y2 -= quadWidth / 2;
        painter.drawRect(x2, y2, quadWidth, quadWidth);
    }

    for(int i = 0; i < pairLengthManipulators.size(); ++i)
    {
        x1 = pairLengthManipulators[i].position.x();
        y1 = pairLengthManipulators[i].position.y();
        x2 = x1 + pairLengthManipulators[i].maxLength * qCos(pairLengthManipulators[i].angle);
        y2 = y1 + pairLengthManipulators[i].maxLength * qSin(pairLengthManipulators[i].angle);
        painter.drawLine(x1, y1, x2, y2);


        if(dragging && (draggingType == Type::PairLengthManipulator) && (i == draggingID) && (draggingPoint == 1))
        {
            painter.setBrush(QBrush(Qt::blue));
        } else {
            painter.setBrush(QBrush(Qt::red));
        }

        x1 = pairLengthManipulators[i].position.x() +
                pairLengthManipulators[i].currentLength1 * qCos(pairLengthManipulators[i].angle) - quadWidth / 2;
        y1 = pairLengthManipulators[i].position.y() +
                pairLengthManipulators[i].currentLength1 * qSin(pairLengthManipulators[i].angle) - quadWidth / 2;
        painter.drawRect(x1, y1, quadWidth, quadWidth);

        if(dragging && (draggingType == Type::PairLengthManipulator) && (i == draggingID) && (draggingPoint == 2))
        {
            painter.setBrush(QBrush(Qt::blue));
        } else {
            painter.setBrush(QBrush(Qt::red));
        }

        x1 = pairLengthManipulators[i].position.x() +
                pairLengthManipulators[i].currentLength2 * qCos(pairLengthManipulators[i].angle) - quadWidth / 2;
        y1 = pairLengthManipulators[i].position.y() +
                pairLengthManipulators[i].currentLength2 * qSin(pairLengthManipulators[i].angle) - quadWidth / 2;
        painter.drawRect(x1, y1, quadWidth, quadWidth);
    }

    for(int i = 0; i < pairAngleManipulators.size(); ++i)
    {
        painter.setPen(QColor(0, 0, 0, 0));
        painter.setBrush(QColor(0, 255, 0));
        if(pairAngleManipulators[i].minAngle <= pairAngleManipulators[i].maxAngle)
        {
            x1 = pairAngleManipulators[i].position.x() - pairAngleManipulators[i].radius;
            y1 = pairAngleManipulators[i].position.y() - pairAngleManipulators[i].radius;

            int startAngle = -pairAngleManipulators[i].minAngle / M_PI * 180.0f * 16.0f;
            int spanAngle = (pairAngleManipulators[i].minAngle - pairAngleManipulators[i].maxAngle) / M_PI * 180.0f * 16.0f;

            painter.drawPie(x1, y1, pairAngleManipulators[i].radius * 2.0f, pairAngleManipulators[i].radius * 2.0f, startAngle, spanAngle);

            // Drawing selected area
            painter.setBrush(Qt::magenta);
            startAngle = -pairAngleManipulators[i].angle1 / M_PI * 180.0f * 16.0f;
            spanAngle = (pairAngleManipulators[i].angle1 - pairAngleManipulators[i].angle2) / M_PI * 180.0f * 16.0f;

            painter.drawPie(x1, y1, pairAngleManipulators[i].radius * 2.0f, pairAngleManipulators[i].radius * 2.0f, startAngle, spanAngle);
        } else {
            x1 = pairAngleManipulators[i].position.x() - pairAngleManipulators[i].radius;
            y1 = pairAngleManipulators[i].position.y() - pairAngleManipulators[i].radius;

            int startAngle = -pairAngleManipulators[i].minAngle / M_PI * 180.0f * 16.0f;
            int spanAngle = -(pairAngleManipulators[i].maxAngle + M_PI * 2.0 - pairAngleManipulators[i].minAngle) / M_PI * 180.0f * 16.0f;

            painter.drawPie(x1, y1, pairAngleManipulators[i].radius * 2.0f, pairAngleManipulators[i].radius * 2.0f, startAngle, spanAngle);

            // Drawing selected area
            painter.setBrush(Qt::magenta);
            startAngle = -pairAngleManipulators[i].angle1 / M_PI * 180.0f * 16.0f;
            if(pairAngleManipulators[i].angle2 >= pairAngleManipulators[i].minAngle)
            {
                spanAngle = (pairAngleManipulators[i].angle1 - pairAngleManipulators[i].angle2) / M_PI * 180.0f * 16.0f;
            } else {
                if(pairAngleManipulators[i].angle1 >= pairAngleManipulators[i].minAngle)
                {
                    spanAngle = -(2.0f * M_PI - pairAngleManipulators[i].angle1 + pairAngleManipulators[i].angle2) / M_PI * 180.0f * 16.0f;
                } else {
                    spanAngle = (pairAngleManipulators[i].angle1 - pairAngleManipulators[i].angle2) / M_PI * 180.0f * 16.0f;
                }
            }

            painter.drawPie(x1, y1, pairAngleManipulators[i].radius * 2.0f, pairAngleManipulators[i].radius * 2.0f, startAngle, spanAngle);
        }
        painter.fillRect(x1, y1, pairAngleManipulators[i].radius * 2.0f, pairAngleManipulators[i].radius * 2.0f, QBrush(QColor(255, 255, 255, 192)));

        painter.setPen(QColor(0, 0, 0));

        if(dragging && (draggingType == Type::PairAngleManipulator) && (i == draggingID) && (draggingPoint == 1))
        {
            painter.setBrush(QBrush(Qt::blue));
        } else {
            painter.setBrush(QBrush(Qt::red));
        }

        x1 = pairAngleManipulators[i].position.x();
        y1 = pairAngleManipulators[i].position.y();

        x2 = x1 + pairAngleManipulators[i].radius * qCos(pairAngleManipulators[i].angle1);
        y2 = y1 + pairAngleManipulators[i].radius * qSin(pairAngleManipulators[i].angle1);

        painter.drawLine(x1, y1, x2, y2);

        x2 -= quadWidth / 2;
        y2 -= quadWidth / 2;
        painter.drawRect(x2, y2, quadWidth, quadWidth);

        if(dragging && (draggingType == Type::PairAngleManipulator) && (i == draggingID) && (draggingPoint == 2))
        {
            painter.setBrush(QBrush(Qt::blue));
        } else {
            painter.setBrush(QBrush(Qt::red));
        }

        x2 = x1 + pairAngleManipulators[i].radius * qCos(pairAngleManipulators[i].angle2);
        y2 = y1 + pairAngleManipulators[i].radius * qSin(pairAngleManipulators[i].angle2);

        painter.drawLine(x1, y1, x2, y2);

        x2 -= quadWidth / 2;
        y2 -= quadWidth / 2;
        painter.drawRect(x2, y2, quadWidth, quadWidth);
    }
}

void VisualEditWidget::manualValueEdit(Type::Manipulator manipulatorType, int id, int currentPoint, double value)
{
    switch(manipulatorType)
    {
    case Type::LengthManipulator:
        break;
    case Type::AngleManipulator:
        angleManipulators[id].angle = angleManipulators[id].minAngle + value / 180.0 * M_PI;
        if(angleManipulators[id].angle >= M_PI * 2.0f)
        {
            angleManipulators[id].angle -= M_PI * 2.0f;
        }
        break;
    case Type::PairLengthManipulator:
        break;
    case Type::PairAngleManipulator:
        if(currentPoint == 1)
        {
            pairAngleManipulators[id].angle1 = pairAngleManipulators[id].minAngle + value / 180.0 * M_PI;
            if(pairAngleManipulators[id].angle1 >= M_PI * 2.0f)
            {
                pairAngleManipulators[id].angle1 -= M_PI * 2.0f;
            }
        } else {
            pairAngleManipulators[id].angle2 = pairAngleManipulators[id].minAngle + value / 180.0 * M_PI;
            if(pairAngleManipulators[id].angle2 >= M_PI * 2.0f)
            {
                pairAngleManipulators[id].angle2 -= M_PI * 2.0f;
            }
        }
        break;
    }
    repaint();
}

void VisualEditWidget::createWidgets()
{
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    okButton = new QPushButton("OK", this);
    layout -> addWidget(okButton, 0, Qt::AlignBottom);

    cancelButton = new QPushButton("Cancel", this);
    layout -> addWidget(cancelButton, 0, Qt::AlignBottom);

    setLayout(layout);
}

void VisualEditWidget::createConnections()
{
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

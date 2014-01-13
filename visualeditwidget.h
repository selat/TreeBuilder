#ifndef VISUALEDITWIDGET_H
#define VISUALEDITWIDGET_H

#include <QtWidgets>

#include "lengthmanipulator.h"
#include "anglemanipulator.h"
#include "visualeditwidgettypes.h"

class VisualEditWidget : public QDialog
{
    Q_OBJECT
public:

    explicit VisualEditWidget(QWidget *parent = 0);
    void setQuadWidth(int value);

    int addLengthManipulator(QPoint position, float angle, float currentLength, float maxLength);
    int addAngleManipulator(QPoint position, float radius, float angle, float minAngle, float maxAngle);

    int addPairLengthManipulator(QPoint position, float angle, float currentLength1, float currentLength2, float maxLength);
    int addPairAngleManipulator(QPoint position, float radius, float angle1, float angle2, float minAngle, float maxAngle);

    LengthManipulator* getLengthManipulator(int id);
    AngleManipulator* getAngleManipulator(int id);
    PairLengthManipulator* getPairLengthManipulator(int id);
    PairAngleManipulator* getPairAngleManipulator(int id);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    
signals:
    
public slots:
    void manualValueEdit(Type::Manipulator manipulatorType, int id, int currentPoint, double value);

private:
    void createWidgets();
    void createConnections();

    const float e;

    Type::Manipulator draggingType;
    int draggingID;
    int draggingPoint;
    bool dragging;
    int quadWidth;

    QVector <LengthManipulator> lengthManipulators;
    QVector <AngleManipulator> angleManipulators;

    QVector <PairLengthManipulator> pairLengthManipulators;
    QVector <PairAngleManipulator> pairAngleManipulators;

    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // VISUALEDITWIDGET_H

#ifndef VISUALEDITTRUNKBENDING_H
#define VISUALEDITTRUNKBENDING_H

#include "visualeditwidget.h"

class VisualEditProcessBending : public VisualEditWidget
{
public:
    VisualEditProcessBending(QWidget *parent = 0);

    double getMinAngle();
    double getMaxAngle();
    void setMinAngle(double value);
    void setMaxAngle(double value);

private:
    int trunkBendingEdit;
};

#endif // VISUALEDITTRUNKBENDING_H

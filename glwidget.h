#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "Tree.h"

class GLWidget : public QGLWidget, public Tree
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);

public slots:
    void setWireframeView(bool state);
    void setShowTrunk(bool state);
    void setShowBranches(bool state);
    void setShowLeaves(bool state);

    void setBackgroundColor();

    void generate();

    void loadTrunkTexture(QString value);
    void loadBranchesTexture(QString value);
    void loadTextures();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent(QKeyEvent *keyEvent);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

    QPoint mousePosition;

    bool wireframeView;
    bool showTrunk;
    bool showBranches;
    bool showLeaves;

    QColor backgroundColor;

    GLfloat translateX, translateY, translateZ;
    GLfloat nSca;
    GLfloat xRot, yRot, zRot;

    GLuint trunkTexture;
    GLuint branchesTexture;
    QVector <GLuint> leavesTextures;
};

#endif // GLWIDGET_H

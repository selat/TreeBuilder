#include "glwidget.h"

#include <QApplication>
#include <QKeyEvent>
#include <QtWidgets>

#include <fstream>

#include <cmath>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent), backgroundColor(Qt::white)
{
    wireframeView = false;
    showTrunk = true;
    showBranches = true;
    showLeaves = true;

    translateX = 0.0f;
    translateY = -10.0f;
    translateZ = -30.0f;

    xRot = 0.0f;
    yRot = 0.0f;
    zRot = 0.0f;

    nSca = 1.0f;

    Tree::setTrunkTexture((QApplication::applicationDirPath() + "/" + "trunk.png").toStdString());
    Tree::setBranchesTexture((QApplication::applicationDirPath() + "/" + "trunk.png").toStdString());

    setFocus();
}

void GLWidget::loadTrunkTexture(QString value)
{
    Tree::setTrunkTexture(value.toStdString());
    loadTextures();
    updateGL();
}

void GLWidget::loadBranchesTexture(QString value)
{
    Tree::setBranchesTexture(value.toStdString());
    loadTextures();
    updateGL();
}

void GLWidget::setWireframeView(bool state)
{
    wireframeView = state;
    updateGL();
}

void GLWidget::setShowTrunk(bool state)
{
    showTrunk = state;
    updateGL();
}

void GLWidget::setShowBranches(bool state)
{
    showBranches = state;
    updateGL();
}

void GLWidget::setShowLeaves(bool state)
{
    showLeaves = state;
    updateGL();
}

void GLWidget::setBackgroundColor()
{
    backgroundColor = QColorDialog::getColor(Qt::white, this, "Color");
    qglClearColor(backgroundColor);
}

void GLWidget::generate()
{
    Tree::clear();
    Tree::generate(0.0, 0.0, 0.0);
    loadTextures();
    updateGL();
}

void GLWidget::initializeGL()
{
    qglClearColor(backgroundColor);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glAlphaFunc(GL_EQUAL, 1.0f);

    loadTextures();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(45.0, (GLdouble)w / (GLdouble)h, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(translateX, translateY, translateZ);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    glScalef(nSca, nSca, nSca);

    if(wireframeView)
    {
        glDisable(GL_TEXTURE_2D);
        if(showTrunk)
        {
            glColor3ub(104, 72, 47);
            for(unsigned int i = 0; i < trunk.size(); ++i)
            {
                glBegin(GL_LINE_LOOP);
                glVertex3f(trunk[i].v[0].x, trunk[i].v[0].y, trunk[i].v[0].z);
                glVertex3f(trunk[i].v[1].x, trunk[i].v[1].y, trunk[i].v[1].z);
                glVertex3f(trunk[i].v[2].x, trunk[i].v[2].y, trunk[i].v[2].z);
                glEnd();
            }
        }
        if(showBranches)
        {
            glColor3ub(104, 72, 47);
            for(unsigned int i = 0; i < branches.size(); ++i)
            {
                glBegin(GL_LINE_LOOP);
                glVertex3f(branches[i].v[0].x, branches[i].v[0].y, branches[i].v[0].z);
                glVertex3f(branches[i].v[1].x, branches[i].v[1].y, branches[i].v[1].z);
                glVertex3f(branches[i].v[2].x, branches[i].v[2].y, branches[i].v[2].z);
                glEnd();
            }
        }
        if(showLeaves)
        {
            glColor3ub(0, 128, 0);
            for(unsigned int i = 0; i < leaves.size(); ++i)
            {
                glBegin(GL_LINE_LOOP);
                    glVertex3f(leaves[i].v[0].x, leaves[i].v[0].y, leaves[i].v[0].z);
                    glVertex3f(leaves[i].v[1].x, leaves[i].v[1].y, leaves[i].v[1].z);
                    glVertex3f(leaves[i].v[2].x, leaves[i].v[2].y, leaves[i].v[2].z);
                    glVertex3f(leaves[i].v[3].x, leaves[i].v[3].y, leaves[i].v[3].z);
                glEnd();
            }
        }
        glEnable(GL_TEXTURE_2D);
    } else {
        if(showTrunk)
        {
            glColor3ub(104, 72, 47);
            glBindTexture(GL_TEXTURE_2D, trunkTexture);
            glBegin(GL_TRIANGLES);
            for(unsigned int i = 0; i < trunk.size(); ++i)
            {
                glTexCoord2f(trunk[i].v[0].u, trunk[i].v[0].v); glVertex3f(trunk[i].v[0].x, trunk[i].v[0].y, trunk[i].v[0].z);
                glTexCoord2f(trunk[i].v[1].u, trunk[i].v[1].v); glVertex3f(trunk[i].v[1].x, trunk[i].v[1].y, trunk[i].v[1].z);
                glTexCoord2f(trunk[i].v[2].u, trunk[i].v[2].v); glVertex3f(trunk[i].v[2].x, trunk[i].v[2].y, trunk[i].v[2].z);
            }
            glEnd();
        }
        if(showBranches)
        {
            glColor3ub(104, 72, 47);
            glBindTexture(GL_TEXTURE_2D, branchesTexture);
            glBegin(GL_TRIANGLES);
            for(unsigned int i = 0; i < branches.size(); ++i)
            {
                glTexCoord2f(branches[i].v[0].u, branches[i].v[0].v); glVertex3f(branches[i].v[0].x, branches[i].v[0].y, branches[i].v[0].z);
                glTexCoord2f(branches[i].v[1].u, branches[i].v[1].v); glVertex3f(branches[i].v[1].x, branches[i].v[1].y, branches[i].v[1].z);
                glTexCoord2f(branches[i].v[2].u, branches[i].v[2].v); glVertex3f(branches[i].v[2].x, branches[i].v[2].y, branches[i].v[2].z);
            }
            glEnd();
        }
        if(showLeaves)
        {
            for(unsigned int i = 0; i < leaves.size(); ++i)
            {
                glBindTexture(GL_TEXTURE_2D, leavesTextures[leaves[i].texture_id]);
                glBegin(GL_QUADS);
                    glTexCoord2f(leaves[i].v[0].u, leaves[i].v[0].v); glVertex3f(leaves[i].v[0].x, leaves[i].v[0].y, leaves[i].v[0].z);
                    glTexCoord2f(leaves[i].v[1].u, leaves[i].v[1].v); glVertex3f(leaves[i].v[1].x, leaves[i].v[1].y, leaves[i].v[1].z);
                    glTexCoord2f(leaves[i].v[2].u, leaves[i].v[2].v); glVertex3f(leaves[i].v[2].x, leaves[i].v[2].y, leaves[i].v[2].z);
                    glTexCoord2f(leaves[i].v[3].u, leaves[i].v[3].v); glVertex3f(leaves[i].v[3].x, leaves[i].v[3].y, leaves[i].v[3].z);
                glEnd();
            }
        }
    }
}

void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent -> key())
    {
    case Qt::Key_Plus:
        translateZ /= 1.1f;
        break;
    case Qt::Key_Minus:
        translateZ *= 1.1f;
        break;
    }
    repaint();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mousePosition = event -> pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    xRot += 180.0f * (GLfloat)(event -> y() - mousePosition.y()) / height();
    yRot += 180.0f * (GLfloat)(event -> x() - mousePosition.x()) / width();
    mousePosition = event -> pos();
    updateGL();
}

void GLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan(fovy * M_PI / 180.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

void GLWidget::loadTextures()
{
    glDeleteTextures(1, &trunkTexture);
    glDeleteTextures(1, &branchesTexture);
    glDeleteTextures(leaves_properties.size(), leavesTextures.begin());
    QImage img1(QString::fromStdString(trunk_texture_name));
    glGenTextures(1, &trunkTexture);
    glBindTexture(GL_TEXTURE_2D, trunkTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)img1.width(), (GLsizei)img1.height(),
                 0, GL_BGRA, GL_UNSIGNED_BYTE, img1.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    QImage img2(QString::fromStdString(branches_texture_name));
    glGenTextures(1, &branchesTexture);
    glBindTexture(GL_TEXTURE_2D, branchesTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)img2.width(), (GLsizei)img2.height(),
                 0, GL_BGRA, GL_UNSIGNED_BYTE, img2.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    leavesTextures.resize(leaves_properties.size());
    glGenTextures(leaves_properties.size(), leavesTextures.begin());
    for(int i = 0; i < leaves_properties.size(); ++i)
    {
        QImage img3(QString::fromStdString(leaves_properties[i].texture));
        glBindTexture(GL_TEXTURE_2D, leavesTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)img3.width(), (GLsizei)img3.height(),
                     0, GL_BGRA, GL_UNSIGNED_BYTE, img3.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }
}

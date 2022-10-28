#ifndef GLVIEW_H
#define GLVIEW_H

#include <QWidget>
#include <QtOpenGL>
#include <QtOpenGLWidgets>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <QTimer>
#include <QMouseEvent>
#include "ui_glview.h"


QT_BEGIN_NAMESPACE
namespace Ui { class glView;
};

QT_END_NAMESPACE

class glView : public QOpenGLWidget
{
    Q_OBJECT

private:
    float x,y,z;
    float xRot,yRot,zRot;
    QPoint mPos;
    QTimer tmr;
    void start();
    void cameraMove();
    void drawLevel(int a);
    void menu();
    void shoot(bool shooter);
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public:
    glView(QWidget *parent = nullptr);
    ~glView();

private:
    Ui::glView ui;

public slots:
    void slotButtonClicked();

};
#endif // GLVIEW_H

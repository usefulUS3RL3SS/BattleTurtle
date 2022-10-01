#include "openglwindow.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
}


OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLWindow::initialize()
{
}

void OpenGLWindow::render()
{
    m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    QPainter painter(m_device);
    render(&painter);
}



#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE


class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = nullptr);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void initialize();

    void render();

private:
    QOpenGLPaintDevice *m_device = nullptr;

};

#endif // WINDOW_H

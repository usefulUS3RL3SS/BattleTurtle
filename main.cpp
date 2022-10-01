#include <QApplication>

#include "openglwindow.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>



class mainWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;
};



int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    mainWindow window;
    window.setFormat(format);
    window.resize(1280, 640);
    window.show();


    return app.exec();
}

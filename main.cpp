#include "glview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    glView glView;
    glView.show();
    return a.exec();
}

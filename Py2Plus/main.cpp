#include "Py2Plus.h"
#include <QtWidgets/QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Py2Plus w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - w.width()) / 2;
        int y = (screenGeometry.height() - w.height()) / 2;
        w.move(x, y);
    }
    w.show();
    return a.exec();
}
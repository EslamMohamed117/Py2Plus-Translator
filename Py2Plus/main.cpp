#include "Py2Plus.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Py2Plus w;
    w.show();
    return a.exec();
}

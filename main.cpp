#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("WuJC");
    QCoreApplication::setApplicationName("Wu's GIS");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    Widget w;
    w.show();
    return a.exec();
}

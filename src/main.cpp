#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute (Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    MainWindow w(new mathVM("/home/ivan/CppProjects/calc/src/mathModel"));
    w.show();
    return a.exec();
}

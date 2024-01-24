#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute (Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    mathVM* vm = new mathVM("/home/ivan/CppProjects/calc/src/mathModel");
    funsModel* fm = new funsModel(vm->getModelRef ());
    varsModel* varsm = new varsModel(vm->getModelRef ());
    QObject::connect(vm->getModelRef(), &mathModel::newVarEvent, varsm, &varsModel::newRow);
    QObject::connect(vm, &mathVM::calculationFinished, varsm, &varsModel::fixNewRow);
    MainWindow w(vm, fm, varsm);
    w.show();
    return a.exec();
}

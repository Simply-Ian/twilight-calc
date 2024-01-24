#include "mainwindow.h"
#include <QApplication>
#include <cstdlib>
#include <filesystem>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    a.setAttribute (Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);

    std::filesystem::path scripts_folder_path = std::getenv("HOME");
    scripts_folder_path /= ".twcalc";
    // "/home/ivan/CppProjects/calc/src/mathModel"
    mathVM* vm = new mathVM(scripts_folder_path);
    funsModel* fm = new funsModel(vm->getModelRef ());
    varsModel* varsm = new varsModel(vm->getModelRef ());
    QObject::connect(vm->getModelRef(), &mathModel::newVarEvent, varsm, &varsModel::newRow);
    QObject::connect(vm, &mathVM::calculationFinished, varsm, &varsModel::fixNewRow);
    MainWindow w(vm, fm, varsm);
    w.show();
    return a.exec();
}

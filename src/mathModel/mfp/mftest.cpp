#include "mathFunProvider.h"
#include <iostream>
#include "exceptions.h"

int main() {
    mathFunProvider MFP;
    auto result = MFP.load_scripts("/home/ivan/CppProjects/calc/");
    double x = result.at("first.first")({2.0});
    try {
        double y = result.at("second.second_A")({5});
    }
    catch (invalidOperation exc) {
        std::cout << "При выполнении функции second.second_A произошла ошибка" << std::endl;
        std::cout << "Недопустимая операция: " << exc.what() << std::endl;
    }
    int exit;
    std::cin >> exit;
    return 0;
}
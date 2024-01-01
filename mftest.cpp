#include "mathFunProvider.h"

int main() {
    mathFunProvider MFP;
    auto result = MFP.load_scripts("/home/ivan/CppProjects/calc/test_scripts");
    double x = result.at("first.first")({2.0});
    return 0;
}
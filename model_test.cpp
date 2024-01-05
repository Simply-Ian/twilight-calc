#include "model.h"
#include <iostream>

int main(int argc, char** argv){
    Model M {"/home/ivan/CppProjects/calc"};
    M.set_var("a", 10);
    M.set_var("b", -0.5);
    if (argc > 1){
        double R = M.calculate_expression(argv[1]);
        std::cout << R << std::endl;
    }
    return 0;
}
//! @file Модуль для расчета математических выражений
#include <string>
#include <iostream> // Только для отладки!
#include "exceptions.h"
#include "compute_RPN.h"
#include "expr_to_RPN.h"
#include "tokenize.h"
#include <cmath>
#include <printf.h>


int main(int argc, char** argv){
    auto print_q = [](tokenq_t q){
        for (int i = q.size(); i > 0; i--){
            std::cout << q.front().value << ' ';
            q.pop();
        }
        std::cout << std::endl;
    };
    std::function<double(std::vector<double>)> fact = [&fact](std::vector<double> a) -> double {
        if (a[0] > 0)
            return a[0] == 2? 2 : a[0] * fact({a[0] - 1});
        else throw std::invalid_argument(std::to_string(a[0]));
    };
    std::map <std::string, mathFun> builtin_funs = {
        {"+", {[](std::vector<double> a) -> double {return a[0] + a[1];}, 2}},
        {"-", {[](std::vector<double> a) -> double {return a[0] - a[1];}, 2}},
        {"*", {[](std::vector<double> a) -> double {return a[0] * a[1];}, 2}},
        {"/", {[](std::vector<double> a) -> double {return a[0] / a[1];}, 2}},
        {"//", {[](std::vector<double> a) -> double {return (a[0] - std::remainder(a[0], a[1])) / a[1];}, 2}},
        {"%", {[](std::vector<double> a) -> double {return std::remainder(a[0], a[1]);}, 2}},
        {"^", {[](std::vector<double> a) -> double {return std::pow(a[0], a[1]);}, 2}},
        {"sin", {[](std::vector<double> a) -> double {return std::sin(a[0]);}, 1}},
        {"cos", {[](std::vector<double> a) -> double {return std::cos(a[0]);}, 1}},
        {"tg", {[](std::vector<double> a) -> double {return std::tan(a[0]);}, 1}},
        {"ctg", {[](std::vector<double> a) -> double {return 1 / std::tan(a[0]);}, 1}},
        {"fact", {fact, 1}},
        {"sqrt", {[](std::vector<double> a) -> double {return std::sqrt(a[0]);}, 1}},
        {"if", {[](std::vector<double> a) -> double {return a[0]? a[1] : a[2];}, 3}}
    };
    std::map <std::string, double> builtin_vars = {
        {"pi", M_PI}, {"e", M_E}
    };
    if (argc > 1){
        std::vector<std::string> builtin_funs_names;
        for (auto fun_pair: builtin_funs)
            builtin_funs_names.push_back(fun_pair.first);
        
        try {
            tokenq_t raw_tokens = tokenize(argv[1]);
            print_q(raw_tokens);
            tokenq_t RPN = expr_to_RPN(raw_tokens, builtin_funs_names, {"pi", "e"});
            print_q(RPN);
            double result = compute_RPN(RPN, builtin_funs, builtin_vars);
            std::printf("%.6f\n", result);
            std::cout << result << std::endl;
        }
        catch (const CalcException& e){
            std::cout << e.what() << std::endl;
            std::cout << argv[1] << std::endl;
            std::cout << std::string(e.pos, '~') << "^" << std::endl;
        }
    }
    return 0;
}
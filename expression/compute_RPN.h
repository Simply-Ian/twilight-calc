#ifndef CALC_COMPUTE_RPN
#define CALC_COMPUTE_RPN

#include "tokenize.h"
#include "math_fun.h"
#include <map>

//! @brief Рассчитывает выражение, записанное в обратной польской записи
double compute_RPN(tokenq_t rpn_expr, std::map<std::string, mathFun> functions, std::map<std::string, double> vars);

#endif
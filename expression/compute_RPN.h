#ifndef CALC_COMPUTE_RPN
#define CALC_COMPUTE_RPN

#include "tokenize.h"
#include "math_fun.h"
#include <map>

//! @brief Рассчитывает выражение, записанное в обратной польской записи
/// @param rpn_expr Выражение в форме обратной польской записи
/// @param funs вектор доступных математических функций
/// @param vars вектор доступных переменных (и констант)
/// @throws unknownToken, notEnoughArgs
double compute_RPN(tokenq_t rpn_expr, const std::map<std::string, mathFun>& funs, const std::map<std::string, double>& vars);

#endif
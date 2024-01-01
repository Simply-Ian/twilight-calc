#ifndef CALC_EXPR_TO_RPN
#define CALC_EXPR_TO_RPN

#include "tokenize.h"
#include <vector>

tokenq_t expr_to_RPN(tokenq_t& in_queue, std::vector<std::string> functions, std::vector<std::string> vars);

#endif
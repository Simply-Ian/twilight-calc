#ifndef CALC_TOKENIZE
#define CALC_TOKENIZE
#include "token.h"
#include <queue>
#include <string>

using tokenq_t = std::queue<Token>;

tokenq_t tokenize(std::string expr);

#endif
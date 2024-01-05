#ifndef CALC_EXPR_TO_RPN
#define CALC_EXPR_TO_RPN

#include "tokenize.h"
#include <vector>
#include <map>

static const std::map<std::string, int> op_priorities = {
                                    {"=", -2},
                                    {"<", -1}, {"<=", -1}, {">", -1}, {">=", -1}, {"==", -1}, {"!=", -1},
                                    {"+", 0}, {"-", 0},
                                    {"*", 1}, {"/", 1}, {"%", 1}, {"//", 1},
                                    {"^", 2}
};

//! @brief Конвертирует очередь токенов в инфиксной нотации в обратную польскую по алгоритму сортировочной станции
//! @param in_queue входная очередь токенов
//! @param functions вектор имен поддерживаемых функций
//! @param vars вектор имен существующих переменных
//! @returns очередь токенов в порядке обратной польской записи
//! @throws unknownToken, noOpeningPar, 
tokenq_t expr_to_RPN(tokenq_t& in_queue, std::vector<std::string> functions, std::vector<std::string> vars);

#endif
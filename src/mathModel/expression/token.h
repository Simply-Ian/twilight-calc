#ifndef CALC_TOKEN
#define CALC_TOKEN

#include <string>

enum tokenType {PAR, OPER, ARG_COMMA, NUM, NAME};

struct Token{
    tokenType type;
    std::string value;
    int pos;
    bool operator == (const Token& tok){
        return type == tok.type && value == tok.value;
    }
};

#endif
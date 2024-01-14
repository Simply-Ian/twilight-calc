#include "expr_to_RPN.h"
#include "exceptions.h"
#include <stack>
#include <algorithm> // Для проверки, находится ли значение в контейнере
#include <functional>

//! @brief Возвращает приоритет оператора tok
//! @param tok токен-оператор (Token::type = tokenType::OPER)
//! @returns приоритет оператора tok
int getPriority(Token& tok, std::function<bool(Token)> isFun){
    if (tok.type == tokenType::OPER && op_priorities.contains(tok.value))
        return op_priorities.at(tok.value);
    else if (isFun(tok)) return 100;
    else {
        throw unknownToken(tok.pos);
        return -1;
    }
}

tokenq_t expr_to_RPN(tokenq_t& in_queue, std::vector<std::string> functions, std::vector<std::string> vars){
    Token cur_token;
    tokenq_t out_queue;
    std::stack<Token> stack;
    const Token open_par {tokenType::PAR, "("};
    // Позиция последнего встреченного имени функции в исходной строке. Нужно хранить для корректного сообщения об
    // отсутствии открывающей скобки
    int last_name_pos = 0;
    auto isFun = [&](Token tok){
        return std::find(functions.begin(), functions.end(), tok.value) != functions.end();
    };
    auto isVar = [&](Token tok){
        return std::find(vars.begin(), vars.end(), tok.value) != vars.end();
    };
    int stack_top_priority;
    int cur_priority;

    while(!in_queue.empty()){
        cur_token = in_queue.front();
        in_queue.pop();
        switch (cur_token.type){
            case tokenType::NUM: {
                out_queue.push(cur_token);
                break;
            }
            case tokenType::NAME: {
                if (isVar(cur_token))
                    out_queue.push(cur_token);
                else if (isFun(cur_token)){
                    /* Это нужно, чтобы при вычислении значения в RPN арифметические операции и функции обрабатывались
                    одинаково */ 
                    cur_token.type = tokenType::OPER;
                    stack.push(cur_token);
                    last_name_pos = cur_token.pos;
                }
                else throw unknownToken(cur_token.pos);
                break;
            }
            case tokenType::ARG_COMMA: {
                while (stack.top().value != "("){
                    if (stack.empty()) throw noOpeningPar(last_name_pos);
                    else {
                        out_queue.push(stack.top());
                        stack.pop();
                    }
                }
                break;
            }
            case tokenType::OPER: {
                cur_priority = getPriority(cur_token, isFun);
                while (!stack.empty() && 
                        stack.top().type == tokenType::OPER && 
                        (getPriority(stack.top(), isFun) >= cur_priority)) {// Все операторы левоассоциативны
                    out_queue.push(stack.top());
                    stack.pop();
                }
                stack.push(cur_token);
                break;
            }
            case tokenType::PAR: {
                if (cur_token.value == "(") stack.push(cur_token);
                else {
                    while (!stack.empty() && stack.top().value != "("){
                        if (stack.empty()) throw noOpeningPar(last_name_pos);
                        else {
                            out_queue.push(stack.top());
                            stack.pop();
                        }
                    }
                    // if (stack.size() > 1){
                    //     stack.pop();
                    //     if (isFun(stack.top())){
                    //         out_queue.push(stack.top());
                    //         stack.pop();
                    //     }
                    // }
                    if (!stack.empty()) // Цикл while гарантирует, что на вершине останется (
                        stack.pop();
                    else throw noOpeningPar(last_name_pos);
                }
                break;
            }
        }
    }
    while (!stack.empty()){
        if (stack.top().value == "(") throw noOpeningPar(last_name_pos);
        out_queue.push(stack.top());
        stack.pop();
    }
    return out_queue;
}

#include <stack>
#include "exceptions.h"
#include "compute_RPN.h"


//! @brief Рассчитывает выражение, записанное в обратной польской записи
double compute_RPN(tokenq_t rpn_expr, std::map<std::string, mathFun> funs, std::map<std::string, double> vars){
    std::stack<double> stack;
    Token cur_token;
    while (!rpn_expr.empty()){
        cur_token = rpn_expr.front();
        rpn_expr.pop();
        switch (cur_token.type){
            case tokenType::NUM: {
                stack.push(atof(cur_token.value.c_str()));
                break;
            }
            case tokenType::NAME: {
                if (vars.contains(cur_token.value)){
                    stack.push(vars.at(cur_token.value));
                }
                else throw unknownToken(cur_token.pos);
                break;
            }
            case tokenType::OPER: {
                if (funs.contains(cur_token.value)){
                    std::vector<double> args;
                    mathFun cur_fun = funs.at(cur_token.value);
                    for (int i = 0; i < cur_fun.get_args_num(); i++){
                        if (stack.empty()){
                            throw notEnoughArgs(cur_token.pos);
                            break;
                        }
                        args.insert(args.begin(), {stack.top()});
                        stack.pop();
                    }
                    stack.push(cur_fun(args));
                }
                else throw unknownToken(cur_token.pos);
                break;
            }
        }
    }
    return stack.top();
}
#include <stack>
#include "exceptions.h"
#include "compute_RPN.h"


//! @brief Рассчитывает выражение, записанное в обратной польской записи
double compute_RPN(tokenq_t rpn_expr, const std::map<std::string, mathFun>& funs, const std::map<std::string, double>& vars){
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
				else throw unknownToken(cur_token.pos, cur_token.value.size());
                break;
            }
            case tokenType::OPER: {
                if (funs.contains(cur_token.value)){
                    std::vector<double> args;
                    mathFun cur_fun = funs.at(cur_token.value);
                    for (int i = 0; i < cur_fun.get_args_num(); i++){
                        if (stack.empty()){
							throw notEnoughArgs(cur_token.pos, cur_token.value.size());
                            break;
                        }
                        args.insert(args.begin(), {stack.top()});
                        stack.pop();
                    }
					if ((cur_token.value == "/" || cur_token.value == "//" || cur_token.value == "%") && args.at(1) == 0)
						    throw zeroDivision(cur_token.pos);
                    stack.push(cur_fun(args));
                }
				else throw unknownToken(cur_token.pos, cur_token.value.size());
                break;
            }
        }
    }
	if (stack.empty())
		throw incorrectExpression();
    return stack.top();
}

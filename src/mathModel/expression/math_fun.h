#ifndef CALC_MATHFUN
#define CALC_MATHFUN
#include <functional>

using common_fun_t = std::function<double (std::vector<double>)>;

class mathFun {
    common_fun_t compute;
    int args_num;
    public:
        int get_args_num(){
            return args_num;
        }
        mathFun(common_fun_t f, int an) : compute(f), args_num(an){}
        double operator ()(std::vector<double> args){
            return compute(args);
        }
};

#endif
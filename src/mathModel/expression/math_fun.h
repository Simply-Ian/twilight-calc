#ifndef CALC_MATHFUN
#define CALC_MATHFUN
#include <functional>
#include <string>

using common_fun_t = std::function<double (std::vector<double>)>;

class mathFun {
    common_fun_t compute;
    int args_num;
    public:
	    int get_args_num() const{
            return args_num;
        }
		mathFun(common_fun_t f, int an, std::string d) : compute(f), args_num(an), description(d){}
        double operator ()(std::vector<double> args){
            return compute(args);
        }
		std::string description;
};

#endif

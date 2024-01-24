#ifndef CALC_MATHFUN
#define CALC_MATHFUN
#include <functional>
#include <string>

using common_fun_t = std::function<double (std::vector<double>)>;

/// @brief Функтор, предоставляющий некоторую математическую функцию (встроенную или пользовательскую)
class mathFun {
	/// @brief Математическая функция. Принимает вектор double, возвращает double
    common_fun_t compute;

	/// @brief Количество принимаемых математической функцией аргументов
    int args_num;
    public:
	    int get_args_num() const{
            return args_num;
        }
		mathFun(common_fun_t f, int an, std::string d) : compute(f), args_num(an), description(d){}
        double operator ()(std::vector<double> args){
            return compute(args);
        }
		/// @brief Описание математической функции
		std::string description;
};

#endif

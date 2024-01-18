#ifndef CALC_MODEL_EXCEPTIONS
#define CALC_MODEL_EXCEPTIONS
#include <exception>

class invalidVarName : public std::exception {
    std::string var_name;
    public:
	    invalidVarName(std::string n) : var_name(n) {}
        const char* what() const noexcept {
            return ("Недопустимое имя переменной: " + var_name).c_str();
        }
};

//struct emptyExpr : public std::exception {
//	const char* message = "Задано пустое выражение";
//	const char* what() const noexcept {
//		return message;
//	}
//};

#endif

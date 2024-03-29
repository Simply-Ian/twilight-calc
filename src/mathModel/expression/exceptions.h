#ifndef EXPR_EXCEPTIONS
#define EXPR_EXCEPTIONS
#include <exception>
#include <string>

class incorrectExpression : public std::exception {

};

//! @brief Базовый класс для всех исключений, связанных с некорректным выражением на входе
class CalcException : public std::exception {
    protected:
        std::string message;
    public:
        CalcException(int p) : pos(p){}
        const char* what() const noexcept{
            return message.c_str();
        }
        int pos;
};

//! @brief Означает недопустимый символ в строке: одинокую запятую, неизвестную кракозябру и т. п.
class invalidChar : public CalcException {
    public:
        invalidChar(char t, int p) : CalcException(p) {
            message = "Недопустимый символ в выражении: " + std::string(1, t);
        }
};

//! @brief Означает наличие непарной закрывающей скобки в строке
class noOpeningPar: public CalcException {
    public:
        noOpeningPar(int fun_name_pos) : CalcException(fun_name_pos){
            message = "Не хватает открывающей скобки в вызове функции:";
        }
};

class unknownToken: public CalcException {
    public:
	    int token_size;
		unknownToken(int pos, int ts) : CalcException(pos), token_size(ts){
            message = "Неизвестный токен";
        }
};

class notEnoughArgs: public CalcException {
    public:
	    int token_size;
		notEnoughArgs(int pos, int ts) : CalcException(pos), token_size(ts){
            message = "Недостаточно аргументов у функции";
        }
};

class zeroDivision : public CalcException {
    public:
	    zeroDivision(int pos) : CalcException(pos) {
			message = "Деление на 0 недопустимо";
		}
};

#endif

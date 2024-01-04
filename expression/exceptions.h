#ifndef EXPR_EXCEPTIONS
#define EXPR_EXCEPTIONS
#include <exception>
#include <string>

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
        int pos;
};

//! @brief Означает наличие непарной закрывающей скобки в строке
class noOpeningPar: public CalcException {
    public:
        noOpeningPar(int fun_name_pos) : CalcException(fun_name_pos){
            message = "Не хватает открывающей скобки в вызове функции:";
        };
};

//! @brief Означает наличие непарной закрывающей скобки в строке
class unknownToken: public CalcException {
    public:
        unknownToken(int pos) : CalcException(pos){
            message = "Неизвестный токен";
        };
};

class notEnoughArgs: public CalcException {
    public:
        notEnoughArgs(int pos) : CalcException(pos){
            message = "Недостаточно аргументов у функции";
        };
};

#endif
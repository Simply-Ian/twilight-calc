#ifndef MFP_EXCEPTIONS
#define MFP_EXCEPTIONS
#include <exception>
#include <string>

/// @brief Означает, что пользовательская функция пытается произвести некорректную операцию
class invalidOperation: public std::exception {
    std::string message;

    public:
        invalidOperation(std::string m) : message(m){}
        const char* what() const noexcept{
            return message.c_str();
        }
};

/// @brief Означает, что пользовательская функция возвращает не double
class badReturnType: public invalidOperation{
    public: badReturnType(std::string m) : invalidOperation(m){}
};

/// @brief Означает, что какой-то .py-модуль не получается импортировать
class failToImport : public invalidOperation {
    public: failToImport(std::string m) : invalidOperation(m){}
};

#endif

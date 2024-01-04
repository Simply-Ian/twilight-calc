#ifndef MFP_EXCEPTIONS
#define MFP_EXCEPTIONS
#include <exception>
#include <string>

class invalidOperation: public std::exception {
    std::string message;

    public:
        invalidOperation(std::string m) : message(m){}
        const char* what() const noexcept{
            return message.c_str();
        }
};

class badReturnType: public invalidOperation{
    public: badReturnType(std::string m) : invalidOperation(m){}
};

#endif
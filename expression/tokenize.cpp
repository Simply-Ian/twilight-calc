#include "tokenize.h"
#include "token.h"
#include "exceptions.h"

tokenq_t tokenize(std::string expr){
    // Посимвольное чтение строки
    tokenq_t result;
    tokenType cur_token_type = tokenType::PAR;
    // Находится ли текущий символ внутри списка аргументов некоторой функции
    bool is_in_fun = false;
    std::string cur_token;
    char ch;

    auto new_token = [&](tokenType new_type, int pos){
        if (cur_token != "")
            result.push({cur_token_type, cur_token, pos});
        cur_token_type = new_type;
        cur_token = ch;
    };

    for (int i = 0; i < expr.size(); i++){
        ch = expr.at(i);
        if (ch == '(' || ch == ')'){
            if (ch == '(' && cur_token_type == tokenType::NAME)
                is_in_fun = true;
            else if (ch == ')' && is_in_fun)
                is_in_fun = false;
            new_token(tokenType::PAR, i);
        }
        else if (0x30 <= ch && ch <= 0x39){
            // Символ является цифрой
            if (cur_token_type == tokenType::NUM)
                cur_token += ch;
            else
                new_token(tokenType::NUM, i);
        }
        else if (ch == ','){
            if (is_in_fun)
                new_token(tokenType::ARG_COMMA, i);
            else if (cur_token_type == tokenType::NUM)
                cur_token += ch;
            else throw invalidChar(ch, i);
        }
        else if (ch == '.'){
            if (cur_token_type == tokenType::NUM || cur_token_type == tokenType::NAME)
                cur_token += ch;
            else throw invalidChar(ch, i);
        }
        else if (ch == '+' || ch == '*' || ch == '%' || ch == '^' || ch == '<' || ch == '>'){
            new_token(tokenType::OPER, i);
        }
        else if (ch == '='){
            if (cur_token == ">" || cur_token == "<" || cur_token == "=" || cur_token == "!") 
                cur_token += ch;
            else 
                new_token(tokenType::OPER, i);
        }
        else if (ch == '!'){
            new_token(tokenType::OPER, i);
        }
        else if (ch == '-'){
            new_token(cur_token_type == tokenType::NAME or cur_token_type == tokenType::NUM or cur_token == ")" ? 
                      tokenType::OPER : tokenType::NUM, i);
        }
        else if ( ch == '/' ){
            if (cur_token == "/")
                cur_token += ch;
            else
                new_token(tokenType::OPER, i);
        }
        else if ((0x41 <= ch && ch <= 0x5a) || (0x61 <= ch && ch <= 0x7a)){
            // Текущий символ -- латинская буква
            if (cur_token_type == tokenType::NAME)
                cur_token += ch;
            else 
                new_token(tokenType::NAME, i);
        }
        else throw invalidChar(ch, i);
    }
    new_token(cur_token_type, expr.size() - 1); // Дописываем последний в строке токен
    return result;
}

#include "tokenize.h"
#include "token.h"
#include "exceptions.h"
#include <clocale>

tokenq_t tokenize(std::string expr){
    // Посимвольное чтение строки
    tokenq_t result;
    tokenType cur_token_type = tokenType::PAR;
    // Находится ли текущий символ внутри списка аргументов некоторой функции
    bool is_in_fun = false;
    std::string cur_token;
    char ch;
	/* В разных локалях разделителем целой и дробной части числа может быть "." или ",". Функция atof,
	   которая используется в модуле compute_RPN, в зависимости от локали игнорирует один из разделителей: например,
	   "2.2" -> 2.0, "2,2" -> 2.2 в русской локали. Так как на вход может прийти дробное число в любом формате, при токенизации
	   заменяем и точку, и запятую на символ, определенный в текущей локали
	*/
	std::string dec_point = localeconv()->decimal_point;

    auto new_token = [&](tokenType new_type, int pos){
        if (cur_token != "")
            result.push({cur_token_type, cur_token, pos});
        cur_token_type = new_type;
        cur_token = ch;
    };
	// Счетчик встреченных пробелов. Нужен для корректного расчета позиций токенов и, следовательно, корректного оповещения об ошибках
	int whsp_counter =  0;

    for (int i = 0; i < expr.size(); i++){
        ch = expr.at(i);
        if (ch == '(' || ch == ')'){
            if (ch == '(' && cur_token_type == tokenType::NAME)
                is_in_fun = true;
            else if (ch == ')' && is_in_fun)
                is_in_fun = false;
			new_token(tokenType::PAR, i - cur_token.size() - whsp_counter);
        }
        else if (0x30 <= ch && ch <= 0x39){
            // Символ является цифрой
            if (cur_token_type == tokenType::NUM || cur_token_type == tokenType::NAME)
                cur_token += ch;
            else
				new_token(tokenType::NUM, i - cur_token.size() - whsp_counter);
        }
        else if (ch == '_'){
			if (cur_token_type == tokenType::NAME)
                cur_token += ch;
			else if (cur_token_type != tokenType::NUM) throw invalidChar(ch, i);
        }
        else if (ch == '\''){
			if (cur_token_type != tokenType::NUM)
				throw invalidChar(ch, i);
        }
        else if (ch == ','){
            if (is_in_fun)
				new_token(tokenType::ARG_COMMA, i - cur_token.size() - whsp_counter);
            else if (cur_token_type == tokenType::NUM)
				cur_token += dec_point;
            else throw invalidChar(ch, i);
        }
        else if (ch == '.'){
			if (cur_token_type == tokenType::NAME)
                cur_token += ch;
			else if (cur_token_type == tokenType::NUM)
				cur_token += dec_point;
            else throw invalidChar(ch, i);
        }
        else if (ch == '+' || ch == '*' || ch == '%' || ch == '^' || ch == '<' || ch == '>'){
			new_token(tokenType::OPER, i - cur_token.size() - whsp_counter);
        }
        else if (ch == '='){
            if (cur_token == ">" || cur_token == "<" || cur_token == "=" || cur_token == "!") 
                cur_token += ch;
			else if (cur_token_type == tokenType::NAME)
				new_token(tokenType::OPER, i - cur_token.size() - whsp_counter);
			else
				throw invalidChar(ch, i);
        }
        else if (ch == '!'){
			new_token(tokenType::OPER, i - cur_token.size() - whsp_counter);
        }
        else if (ch == '-'){
            new_token(cur_token_type == tokenType::NAME or cur_token_type == tokenType::NUM or cur_token == ")" ? 
			          tokenType::OPER : tokenType::NUM, i - cur_token.size() - whsp_counter);
        }
        else if ( ch == '/' ){
            if (cur_token == "/")
                cur_token += ch;
            else
				new_token(tokenType::OPER, i - cur_token.size() - whsp_counter);
        }
        else if ((0x41 <= ch && ch <= 0x5a) || (0x61 <= ch && ch <= 0x7a)){
            // Текущий символ -- латинская буква
            if (cur_token_type == tokenType::NAME)
                cur_token += ch;
            else 
				new_token(tokenType::NAME, i - cur_token.size() - whsp_counter);
        }
		else if (ch == ' ')
			whsp_counter += 1; // Игнорируем пробелы
		else throw invalidChar(ch, i);
		if (ch != ' ') whsp_counter = 0;
    }
	new_token(cur_token_type, expr.size() - cur_token.size()); // Дописываем последний в строке токен
    return result;
}

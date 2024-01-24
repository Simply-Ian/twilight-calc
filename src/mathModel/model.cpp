#include "model.h"
#include "exceptions.h"
#include "expression/exceptions.h"
#include "expression/compute_RPN.h"
#include "expression/expr_to_RPN.h"
#include "expression/tokenize.h"

mathModel::mathModel(std::string script_folder) {
    auto external_functions = MFP.load_scripts(script_folder);
    for (auto fun_pair: external_functions) 
        funs.insert(fun_pair);

    for (auto fun_pair: funs)
        funs_names.push_back(fun_pair.first);
}

double mathModel::get_var(std::string name) const {
    return vars.at(name);
}

void mathModel::set_var(std::string name, double val) {
    for (char ch : name){
        // Если символ не является латинской буквой
        if (!((0x41 <= ch && ch <= 0x5a) || (0x61 <= ch && ch <= 0x7a))) {
            throw invalidVarName(name);
        }
    }
    if (!(name == "pi" || name == "e")) {
        if (!vars.contains(name))
            emit newVarEvent ();
        vars[name] = val;
    }
    else throw constAssign(name);
}

const history_t& mathModel::view_history() const {
    return history;
}

double mathModel::calculate_expression(std::string expression) {
    tokenq_t raw_tokens = tokenize(expression);

	// Обработка присваивания
	std::string var_to_assign = "";
	if (raw_tokens.size() >= 3) {
		tokenq_t queue_to_examine = raw_tokens;
		Token first = queue_to_examine.front();
		queue_to_examine.pop();
		Token second = queue_to_examine.front();
		queue_to_examine.pop();
		if (first.type == tokenType::NAME && second.value == "=") {
			raw_tokens = queue_to_examine;
			var_to_assign = first.value;
		}
	}

    std::vector<std::string> var_names;
    for (auto var_pair: vars) var_names.push_back(var_pair.first);

    tokenq_t RPN = expr_to_RPN(raw_tokens, funs_names, var_names);
    double result = compute_RPN(RPN, funs, vars);
	if (!var_to_assign.empty())
        set_var(var_to_assign, result);

    history.push_back({expression, result});
    return result;
}

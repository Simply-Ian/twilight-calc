#ifndef CALC_MODEL
#define CALC_MODEL
#undef slots // Иначе импорт функций, использующих Python C API, вызовет множество ошибок сборки
#include "mfp/mathFunProvider.h"
#include "mfp/exceptions.h"
#define slots
#define STDFUN1(fun, d) {[](std::vector<double> a) -> double {return fun(a[0]);}, 1, d}
#define STDFUN2(fun, d) {[](std::vector<double> a) -> double {return fun(a[0], a[1]);}, 2, d}
#include <QObject>

using history_t = std::vector<std::pair<std::string, double>>;

/// @brief 
class mathModel : public QObject{
	Q_OBJECT
private:
    mathFunProvider MFP;

    /// @brief Словарь доступных переменных
    std::map<std::string, double> vars = {
        {"pi", M_PI},
        {"e", M_E}
    };

    /// @brief Вектор пар "выражение-результат". Хранит предыдущие выражения, вычисленные в данном сеансе работы с программой
    history_t history;

    /// @brief Словарь всех доступных математических функций и операторов
    std::map <std::string, mathFun> funs {
		{"+", {[](std::vector<double> a) -> double {return a[0] + a[1];}, 2, ""}},
		{"-", {[](std::vector<double> a) -> double {return a[0] - a[1];}, 2, ""}},
		{"*", {[](std::vector<double> a) -> double {return a[0] * a[1];}, 2, ""}},
		{"/", {[](std::vector<double> a) -> double {return a[0] / a[1];}, 2, ""}},
		{"//", {[](std::vector<double> a) -> double {return (a[0] - std::fmod(a[0], a[1])) / a[1];}, 2, ""}},
		{"%", STDFUN2(std::fmod, "")},
		{"^", STDFUN2(std::pow, "")},
		{">", {[](std::vector<double> a) -> double {return a[0] > a[1];}, 2, ""}},
		{">=", {[](std::vector<double> a) -> double {return a[0] >= a[1];}, 2, ""}},
		{"<", {[](std::vector<double> a) -> double {return a[0] < a[1];}, 2, ""}},
		{"<=", {[](std::vector<double> a) -> double {return a[0] <= a[1];}, 2, ""}},
		{"==", {[](std::vector<double> a) -> double {return a[0] == a[1];}, 2, ""}},
		{"!=", {[](std::vector<double> a) -> double {return a[0] != a[1];}, 2, ""}},
		{"sin", STDFUN1(std::sin, "Вычисляет синус угла a (рад)")},
		{"cos", STDFUN1(std::cos, "Вычисляет косинус угла a (рад)")},
		{"tg", STDFUN1(std::tan, "Вычисляет тангенс угла a (рад)")},
		{"ctg", {[](std::vector<double> a) -> double {return 1 / std::tan(a[0]);}, 1, "Вычисляет тангенс угла a (рад)"}},
		{"arcsin", STDFUN1(std::asin, "Вычисляет арксинус угла a (рад)")},
		{"arccos", STDFUN1(std::acos, "Вычисляет арккосинус угла a (рад)")},
		{"arctg", STDFUN1(std::atan, "Вычисляет арктангенс угла a (рад)")},
		{"arcctg", {[](std::vector<double> a) -> double {return 1 / std::atan(a[0]);}, 1, "Вычисляет арккотангенс угла a (рад)"}},
		{"sqrt", STDFUN1(std::sqrt, "Вычисляет квадратный корень из a")},
		{"ln", STDFUN1(std::log, "Вычисляет натуральный логарифм от а")},
		{"log10", STDFUN1(std::log10, "Вычисляет десятичный логарифм от а")},
		{"log", {[](std::vector<double> a) -> double {return std::log(a[1]) / std::log(a[0]);}, 2, "Вычисляет логарифм b по основанию а"}},
		{"if", {[](std::vector<double> a) -> double {return a[0]? a[1] : a[2];}, 3, "Возвращает c, если а равно нулю, иначе возвращает b"}}
    };

    /// @brief Список имен математических функций.
    std::vector<std::string> funs_names;

public:
        /// @brief Возвращает значение заданной переменной
        /// @param name имя переменной
        /// @return численное значение переменной
        /// @throws std::out_of_range, если переменной с заданным именем не существует
        double get_var(std::string name) const;

        /// @todo Назначить на оператор = или обрабатывать на этапе форматирования строки
        /// @brief Назначает значение переменной
        /// @details Присваивает переменной с названием name значение val. Если переменной с таким названием не существует,
        /// создает новую
        /// @throws invalidVarName, если имя переменной содержит символы, отличные от букв или имя обозначает математическую
        /// константу (pi или е)
        void set_var(std::string name, double val);

        /// @brief Вычисляет значение заданного выражения. 
        /// @details Не обрабатывает присваивания
        /// @param expression Строка выражения с вычищенными пробелами и разделителями чисел
        /// @return численное значение выражения.
		/// @throws emptyExpr, unknownToken, notEnoughArgs, noOpeningPar, invalidChar
        double calculate_expression(std::string expression);
        
        /// @brief Позволяет просмотреть историю вычислений
        /// @return Возвращает список предыдущих вычисленных выражений 
        /// @attention Скорее всего, в дальнейшем будет перенесено в отдельную модель
        const history_t& view_history() const;

        mathModel(std::string script_folder);

        /// @returns Константную ссылку на словарь функций
		const std::map<std::string, mathFun>& get_mathfuns_map() const {
			return funs;
		}

        /// @returns Константную ссылку на словарь переменных
		const std::map<std::string, double>& get_vars_map() const {
			return vars;
		}
signals:
    /// @brief Оповещает о том, что результат текущего выражения присвоен новой переменной
    /// @details Позволяет обертке varsModel корректно обработать создание новой переменной и обновить свое View
	void newVarEvent();
};

#endif

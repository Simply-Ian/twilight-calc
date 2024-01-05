#ifndef CALC_MODEL
#define CALC_MODEL
#include "mfp/mathFunProvider.h"
#include "mfp/exceptions.h"
#include "settings.h"
#define STDFUN1(fun) {[](std::vector<double> a) -> double {return fun(a[0]);}, 1}
#define STDFUN2(fun) {[](std::vector<double> a) -> double {return fun(a[0], a[1]);}, 2}

using history_t = std::vector<std::pair<std::string, double>>;

/// @brief 
class Model {
    mathFunProvider MFP;
    std::map<std::string, double> vars = {
        {"pi", M_PI},
        {"e", M_E}
    };
    history_t history;
    std::map <std::string, mathFun> funs {
        {"+", {[](std::vector<double> a) -> double {return a[0] + a[1];}, 2}},
        {"-", {[](std::vector<double> a) -> double {return a[0] - a[1];}, 2}},
        {"*", {[](std::vector<double> a) -> double {return a[0] * a[1];}, 2}},
        {"/", {[](std::vector<double> a) -> double {return a[0] / a[1];}, 2}},
        {"//", {[](std::vector<double> a) -> double {return (a[0] - std::remainder(a[0], a[1])) / a[1];}, 2}},
        {"%", {[](std::vector<double> a) -> double {return std::remainder(a[0], a[1]);}, 2}},
        {"^", {[](std::vector<double> a) -> double {return std::pow(a[0], a[1]);}, 2}},
        {"sin", {[](std::vector<double> a) -> double {return std::sin(a[0]);}, 1}},
        {"cos", {[](std::vector<double> a) -> double {return std::cos(a[0]);}, 1}},
        {"tg", {[](std::vector<double> a) -> double {return std::tan(a[0]);}, 1}},
        {"ctg", {[](std::vector<double> a) -> double {return 1 / std::tan(a[0]);}, 1}},
        {"arcsin", STDFUN1(std::asin)},
        {"arccos", STDFUN1(std::acos)},
        {"arctg", STDFUN1(std::atan)},
        {"arcctg", {[](std::vector<double> a) -> double {return 1 / std::atan(a[0]);}, 1}},
        {"sqrt", {[](std::vector<double> a) -> double {return std::sqrt(a[0]);}, 1}},
        {"if", {[](std::vector<double> a) -> double {return a[0]? a[1] : a[2];}, 3}},
        {">", {[](std::vector<double> a) -> double {return a[0] > a[1];}, 2}},
        {">=", {[](std::vector<double> a) -> double {return a[0] >= a[1];}, 2}},
        {"<", {[](std::vector<double> a) -> double {return a[0] < a[1];}, 2}},
        {"<=", {[](std::vector<double> a) -> double {return a[0] <= a[1];}, 2}},
        {"==", {[](std::vector<double> a) -> double {return a[0] == a[1];}, 2}},
        {"!=", {[](std::vector<double> a) -> double {return a[0] != a[1];}, 2}},
    };

    /// @brief Список имен математических функций.
    std::vector<std::string> funs_names;

    /// @brief Структура, хранящая настройки приложения.
    /// @attention Скорее всего, это поле будет перенесено в отдельную модель
    settings sets;

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
        /// @throws unknownToken, notEnoughArgs, noOpeningPar, invalidChar
        double calculate_expression(std::string expression);
        
        /// @brief Позволяет просмотреть историю вычислений
        /// @return Возвращает список предыдущих вычисленных выражений 
        /// @attention Скорее всего, в дальнейшем будет перенесено в отдельную модель
        const history_t& view_history() const;

        /// @brief Позволяет просмотреть текущие настройки приложения
        /// @returns Ссылку на экземпляр структуры, описывающей настройки
        /// @attention Скорее всего, это поле будет перенесено в отдельную модель
        const settings& get_settings() const;

        /// @brief Сохраняет измененные пользователем настройки
        /// @param new_settings Ссылка на экземпляр структуры, описывающей настройки.
        /// @attention Скорее всего, это поле будет перенесено в отдельную модель
        void save_settings(const settings& new_settings);
        Model(std::string script_folder);
};

#endif
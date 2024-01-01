#ifndef CALC_MATHFUN_PROVIDER
#define CALC_MATHFUN_PROVIDER

#include <Python.h>
#include "expression/math_fun.h"
#include <map>
#include <filesystem>
#include <vector>

/// @brief Запускает окружение Python, загружает пользовательские функции при создании; 
/// завершает работу с окружением Python при уничтожении, реализуя таким образом принцип RAII
class mathFunProvider {
    std::vector<PyObject*> loaded_funs;
    static PyObject* vector_to_py_list(std::vector<double> source);

    /// @brief Загружает одну функцию с заданным именем из модуля
    /// @param fun Python-функция, которую нужно загрузить
    /// @param module_name название модуля
    /// @returns Пару <название функции>, <функтор>
    std::pair<std::string, mathFun> load_new_fun(PyObject* fun, std::string module_name);

    PyObject* builtins;
    
    public:
        mathFunProvider();
        ~mathFunProvider();

        /// @brief Загружает пользовательские функции, определенные в .py-модулях, лежащих в заданной папке.
        /// Python-функции должны принимать на вход список вещественных чисел (List[float]) и возвращать float
        /// @param script_folder папка, в которой хранятся Python-модули
        /// @return Словарь, в котором названию каждой Python-функции соответствует объект mathFun
        std::map <std::string, mathFun> load_scripts(std::filesystem::path script_folder);
};

#endif
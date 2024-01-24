#ifndef CALC_MATHFUN_PROVIDER
#define CALC_MATHFUN_PROVIDER

#include <Python.h>
#include "../expression/math_fun.h"
#include <map>
#include <filesystem>
#include <vector>
#include <initializer_list>
#include <exception>

/// @brief Запускает окружение Python, загружает пользовательские функции при создании; 
/// завершает работу с окружением Python при уничтожении, реализуя таким образом принцип RAII
class mathFunProvider {
    std::vector<PyObject*> loaded_funs;

    /// @brief Создает новый Python-список и помещает в него все элементы source
    /// @param source Вектор со значениями примитивного типа (здесь double), которые нужно поместить в Python-список
    /// @return Указатель на список Python, содержащий все значения из source
    static PyObject* vector_to_py_list(std::vector<double> source);

    /// @brief Обертка над PyObject_CallObject. Помещает все args в кортеж Python, вызывает заданную функцию, возвращает ответ
    /// @param fun Python-функция
    /// @param args Список аргументов Python-функции
    /// @warning Не выполняет проверку PyCallable_Check. Попытка вызвать будет произведена, даже если fun не является callable
    /// @return Значение, возвращенное вызванной Python-функцией
    static PyObject* PyCall(PyObject* fun, std::initializer_list<PyObject*> args);

    /// @brief Обработчик Python-исключений
    /// @details Проверяет, не возникло ли Python-исключение заданного типа и, если возникло, создает C++-исключение заданного типа,
    /// передавая ему в качестве аргумента сообщение об ошиьбке в формате std::string, и выбрасывает его
    /// @param py_exc_type 
    /// @throws C++-исключение заданного типа
    template <typename T>
    static void handle_py_exception(PyObject* py_exc_type);

    /// @brief Оборачивает Python-функцию в C++-лямбду и сохраняет ее в mathFunProvider::loaded_funs
    /// @details Функция принимает уже загруженную из Python-модуля пользовательскую функцию. Для Python-функции находится 
    /// количество принимаемых аргументов. Python-функция оборачивается в C++-код, который занимается приведением типов 
    /// аргументов, обработкой Python-исключений, преобразованием ответа. Из собственного имени функции и названия модуля
    /// собирается название, под которым функция будет доступна пользователю. После этого функция сохраняется 
    /// в mathFunProvider::loaded_funs
    /// @param fun Python-функция, которую нужно сохранить
    /// @param module_name название модуля
    /// @returns Пару <название функции>, <функтор>
    std::pair<std::string, mathFun> load_new_fun(PyObject* fun, std::string module_name);

    /// @brief Python-словарь, содержащий все встроенные функции, классы и переменные языка.
    PyObject* builtins;
    
    public:
        mathFunProvider();
        ~mathFunProvider();

        /// @brief Загружает пользовательские функции, определенные в .py-модулях, лежащих в заданной папке.
        /// @details Python-функции должны принимать на вход список вещественных чисел (List[float]) и возвращать float
        /// @param script_folder папка, в которой хранятся Python-модули
        /// @return Словарь, в котором названию каждой Python-функции соответствует объект mathFun
        std::map <std::string, mathFun> load_scripts(std::filesystem::path script_folder);
};

#endif

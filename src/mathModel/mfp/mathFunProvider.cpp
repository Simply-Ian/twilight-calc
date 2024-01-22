#include <Python.h>
#define PY_SSIZE_T_CLEAN
#include "mathFunProvider.h"
#include <format>
#include "exceptions.h"
#include <csignal>
#include <iostream>
#define PyUnicode_ToStr(p) PyBytes_AsString(PyUnicode_AsASCIIString(p))

bool Py_FinalFlag = false;

mathFunProvider::mathFunProvider(){
    Py_Initialize();
    builtins = PyEval_GetBuiltins();
    // Необходимо для задания интерпретатору Python директории с пользовательскими функциями в mathFunProvider::load_scripts
    PyRun_SimpleString("import sys"); 
}

mathFunProvider::~mathFunProvider(){
    Py_XDECREF(builtins);
    for (PyObject* fun: loaded_funs)
        Py_XDECREF(fun);
    Py_FinalFlag = true;
    // Py_FinalizeEx();  // Может случайно вызвать SEGFAULT 
}

PyObject* mathFunProvider::vector_to_py_list(std::vector<double> source){
    PyObject* R = PyList_New(0);
    for (double item: source)
        PyList_Append(R, PyFloat_FromDouble(item));
    Py_XINCREF(R);
    return R;
}

template <typename T>
void mathFunProvider::handle_py_exception(PyObject* py_exc_type){
    PyObject* possible_error = PyErr_Occurred();
    if (possible_error and PyErr_ExceptionMatches(possible_error)){
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        std::string pStrErrorMessage = PyUnicode_ToStr(PyObject_Str(pvalue));
        PyErr_Clear();
        throw T(pStrErrorMessage);
    }
}

PyObject* mathFunProvider::PyCall(PyObject* fun, std::initializer_list<PyObject*> args){
    PyObject* args_tuple = PyTuple_New(args.size());
    for (auto arg = args.begin(); arg != args.end(); arg++)
        PyTuple_SetItem(args_tuple, arg - args.begin(), *arg);
    PyObject* R = PyObject_CallObject(fun, args_tuple);
    Py_XDECREF(args_tuple);
    // Py_XINCREF(R);
    return R;
}

std::pair<std::string, mathFun> mathFunProvider::load_new_fun(PyObject* fun, std::string module_name){
    // Represents __code__ attr of a Python function
    PyObject* code_attr = PyObject_GetAttrString(fun, "__code__");
    int args_num = PyLong_AsLong(PyObject_GetAttrString(code_attr, "co_argcount"));
    Py_XDECREF(code_attr);

    Py_XINCREF(fun); // Увеличиваем счетчик ссылок, чтобы Py_DECREF в load_scripts не удалил объект, на который ссылается fun
    loaded_funs.push_back(fun);

    auto callback = std::bind([](PyObject* _fun, std::vector<double> args) -> double {
        PyObject* converted_args = vector_to_py_list(args);
        PyObject* R = PyCall(_fun, {converted_args});
        Py_XDECREF(converted_args);
        handle_py_exception<invalidOperation>(PyExc_TypeError);
        double answer = PyFloat_AsDouble(R);
        Py_XDECREF(R);
        handle_py_exception<badReturnType>(PyExc_ValueError);
        return answer;
    }, loaded_funs.back(), std::placeholders::_1);

    PyObject* fun_name = PyObject_GetAttrString(fun, "__name__");
    char* fun_name_decoded =  PyUnicode_ToStr(fun_name);
    Py_XDECREF(fun_name);
    return {module_name + "." + fun_name_decoded,
		mathFun(callback, args_num, "")};
}

std::map <std::string, mathFun> mathFunProvider::load_scripts(std::filesystem::path script_folder){
    std::map <std::string, mathFun> R;
    // Задаем интерпретатору и файловой системе текущую директорию, в которой будем искать скрипты
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::current_path(script_folder);
    PyRun_SimpleString(std::format("sys.path.append('{}')", std::filesystem::current_path().c_str()).c_str());

    PyObject* const PY_SLASH_STR = PyUnicode_FromString("/");
    PyObject* const PY_DOT_STR = PyUnicode_FromString(".");
    PyObject* const dir = PyDict_GetItemString(builtins, "dir");
    for (auto const& dir_item: std::filesystem::recursive_directory_iterator(script_folder)){
        if (dir_item.path().extension() == ".py") {
            std::string module_name = dir_item.path().stem().string();
            // Найдя очередной .py-файл, импортируем его
            std::string dir_item_path = dir_item.path().parent_path() / module_name;
            dir_item_path = dir_item_path.substr(script_folder.string().size() + 1);
            PyObject* cur_module_path = PyUnicode_FromString(dir_item_path.c_str());
            cur_module_path = PyUnicode_Replace(cur_module_path, PY_SLASH_STR, PY_DOT_STR, -1);
            PyObject* cur_module = PyImport_Import(cur_module_path);
            if (!cur_module){
                throw failToImport(dir_item_path);
            }

            /* Получаем список всех имен, определенных в данном модуле и словарь, в котором эти имена соответствуют 
            функциям / классам и т. п.*/
            PyObject** module_dict = _PyObject_GetDictPtr(cur_module);
            if (!module_dict) throw "Failed to get module.__dict__";
            PyObject* module_names = PyCall(dir, {cur_module}); // Все имена, определенные в модуле
            if (!module_names) throw "Failed to get list of names defined in the module " + module_name;

            // Перебираем все имена, определенные в модуле...
            int module_names_len = PyList_Size(module_names);
            for (int i = 0; i < module_names_len; i++){
                PyObject* cur_attr_name = PyList_GetItem(module_names, i);
                Py_XINCREF(cur_attr_name);

                // ... для каждого из них выясняем, не является ли оно built-in (в таком случае имя начинается с __)...
                PyObject* name_prefix;
                if (PyUnicode_GetLength(cur_attr_name) >= 2)
                    name_prefix = PyUnicode_Substring(cur_attr_name, 0, 2);
                if (PyUnicode_GetLength(cur_attr_name) < 2 
                    or PyUnicode_CompareWithASCIIString(name_prefix, "__")){
                    // ...проверяем, обозначает ли имя функцию, а не что-либо иное...
                    PyObject* possible_fun = PyDict_GetItem(*module_dict, cur_attr_name);
                    Py_XINCREF(possible_fun);
                    if (PyFunction_Check(possible_fun)) 
                        // ...и, наконец, оборачиваем эту функцию в C++-код и сохраняем в loaded_funs
                        R.insert(load_new_fun(possible_fun, module_name));
                    Py_XDECREF(possible_fun);
                }
                Py_XDECREF(cur_attr_name);
                Py_XDECREF(name_prefix);
            }
            Py_XDECREF(module_names);
            Py_XDECREF(*module_dict);
            Py_XDECREF(cur_module_path);
            Py_XDECREF(cur_module);
        }
    }
    Py_XDECREF(PY_SLASH_STR);
    Py_XDECREF(PY_DOT_STR);
    Py_XDECREF(dir);
    std::filesystem::current_path(cwd);
    return R;
}

#include <Python.h>
#define PY_SSIZE_T_CLEAN
#include "mathFunProvider.h"
#include <format>
#define PyUnicode_ToStr(p) PyBytes_AsString(PyUnicode_AsASCIIString(p))

mathFunProvider::mathFunProvider(){
    Py_Initialize();
    builtins = PyEval_GetBuiltins();
    PyRun_SimpleString("import sys");
}

mathFunProvider::~mathFunProvider(){
    Py_XDECREF(builtins);
    Py_Finalize();
}

PyObject* mathFunProvider::vector_to_py_list(std::vector<double> source){
    PyObject* R = PyList_New(0);
    for (double item: source)
        PyList_Append(R, PyFloat_FromDouble(item));
    Py_XINCREF(R);
    return R;
}

std::pair<std::string, mathFun> mathFunProvider::load_new_fun(PyObject* fun, std::string module_name){
    // Represents __code__ attr of a Python function
    PyObject* code_attr = PyObject_GetAttrString(fun, "__code__");
    int args_num = PyLong_AsLong(PyObject_GetAttrString(code_attr, "co_argcount"));
    Py_XINCREF(fun);
    loaded_funs.push_back(fun);
    auto callback = std::bind([](PyObject* _fun, std::vector<double> args) -> double {
        PyObject* converted_args = vector_to_py_list(args);
        PyObject* args_tuple = PyTuple_New(1);
        PyTuple_SetItem(args_tuple, 0, converted_args);
        PyObject* R = PyObject_CallObject(_fun, args_tuple);
        Py_XDECREF(converted_args);
        Py_XDECREF(args_tuple);
        double answer = PyFloat_AsDouble(R);
        Py_XDECREF(R);
        return answer;
    }, loaded_funs.back(), std::placeholders::_1);
    PyObject* fun_name = PyObject_GetAttrString(fun, "__name__");
    char* fun_name_decoded =  PyUnicode_ToStr(fun_name);
    Py_XDECREF(fun_name);
    Py_XDECREF(code_attr);
    return {module_name + "." + fun_name_decoded,
        mathFun(callback, args_num)};
}

std::map <std::string, mathFun> mathFunProvider::load_scripts(std::filesystem::path script_folder){
    std::map <std::string, mathFun> R;
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::current_path(script_folder);
    PyRun_SimpleString(std::format("sys.path.append('{}')", std::filesystem::current_path().c_str()).c_str());
    PyObject* const PY_SLASH_STR = PyUnicode_FromString("/");
    PyObject* const PY_DOT_STR = PyUnicode_FromString(".");
    PyObject* const dir = PyDict_GetItemString(builtins, "dir");
    for (auto const& dir_item: std::filesystem::recursive_directory_iterator(script_folder)){
        if (dir_item.path().extension() == ".py") {
            PyObject* cur_module_path = PyUnicode_FromString(dir_item.path().stem().c_str());
            cur_module_path = PyUnicode_Replace(cur_module_path, PY_SLASH_STR, PY_DOT_STR, -1);
            PyObject* cur_module = PyImport_Import(cur_module_path);
            PyObject** module_dict = _PyObject_GetDictPtr(cur_module);
            if (!module_dict) throw "Failed to get module.__dict__";

            PyObject* dir_args = PyTuple_New(1);
            PyTuple_SetItem(dir_args, 0, cur_module);
            PyObject* module_names = PyObject_CallObject(dir, dir_args); // Все имена, определенные в модуле
            Py_XDECREF(dir_args);

            if (!module_names) throw "Failed to get list of names defined in the module " + dir_item.path().stem().string();

            int module_names_len = PyList_Size(module_names);
            for (int i = 0; i < module_names_len; i++){
                PyObject* cur_attr_name = PyList_GetItem(module_names, i);
                Py_XINCREF(cur_attr_name);
                PyObject* name_prefix;
                if (PyUnicode_GetLength(cur_attr_name) >= 2)
                    name_prefix = PyUnicode_Substring(cur_attr_name, 0, 2);
                if (PyUnicode_GetLength(cur_attr_name) < 2 
                    or PyUnicode_CompareWithASCIIString(name_prefix, "__")){
                    PyObject* possible_fun = PyDict_GetItem(*module_dict, cur_attr_name);
                    Py_XINCREF(possible_fun);
                    if (PyFunction_Check(possible_fun)) 
                        R.insert(load_new_fun(possible_fun, PyUnicode_ToStr(cur_attr_name)));
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
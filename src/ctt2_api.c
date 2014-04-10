#undef _DEBUG
#include <Python.h>

#include "system/ctt2.h"
#include "ctt2_api.h"

char* main_py;

static PyObject* _py_tick;

void api_init() {

    PyObject *pName, *pModule, *pDict;

    pName = PyString_FromString("py-scr.main");
    pModule = PyImport_Import(pName);
    if(pModule == 0){
            if(PyErr_Occurred())
                PyErr_Print();
            getch();
            exit(1);
    }
    _py_tick = PyObject_GetAttrString(pModule,"tick");
}

void api_tick() {
}

void api_drop() {
}

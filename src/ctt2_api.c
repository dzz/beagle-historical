#include <Python.h>
#include "ctt2_api.h"
#include "system/ctt2.h"

char* main_py;

static PyObject* _py_tick;

static PyMethodDef 

void api_init() {

    PyObject *pName, *pModule, *pDict;

    pName = PyString_FromString("py/main.py");
    pModule = PyImport_Import(pName);

    _py_tick = PyObject_GetAttrString(pModule,"tick");

}

void api_tick() {
}

void api_drop() {
    free(main_py);
}

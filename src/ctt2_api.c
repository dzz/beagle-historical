#undef _DEBUG
#include <stdio.h>
#include <conio.h>
#include <Python.h>

#include "system/ctt2.h"
#include "ctt2_api.h"

char* main_py;

typedef struct {
    PyObject* __module;
    PyObject* init;
    PyObject* tick;
    PyObject* dispatch_mouseup;
    PyObject* dispatch_mousedown;
    PyObject* dispatch_mousemotion;
    PyObject* dispatch_key;
    PyObject* finalize;
} API_FUNCS;

static API_FUNCS api;


void api_fail_hard() {
    if(PyErr_Occurred())
        PyErr_Print();
    printf("press a key...\n");
    getch();
    exit(1);
}

int api_checkfailure() {
    if(PyErr_Occurred())
        return API_NOFAILURE;
    return 0;
}

#define PY_NOARGS 0
int _pycall_noargs(PyObject* func) {
    if(func && PyCallable_Check( func )) {
        PyObject_CallObject(api.tick,PY_NOARGS);
        return api_checkfailure();
    } 
    return API_FAILURE;
}
#undef PY_NOARGS

int api_init() {
    PyObject *pName, *pModule, *pDict;
    pName = PyString_FromString("py-scr.main");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if(pModule == 0) goto pyfailure;
    api.__module = pModule;
    #define LOAD_PY(x,y) api.##x = PyObject_GetAttrString(api.__module,y); if(api.##x==0) goto pyfailure;
    LOAD_PY(init,"init")
    LOAD_PY(tick,"tick")
    LOAD_PY(dispatch_mouseup,"dispatch_mouseup");
    LOAD_PY(dispatch_mousedown,"dispatch_mousedown");
    LOAD_PY(dispatch_mousemotion,"dispatch_mousemotion");
    LOAD_PY(dispatch_key,"dispatch_key");
    LOAD_PY(finalize,"finalize");
    #undef LOAD_PY
    return _pycall_noargs(api.init);
pyfailure:
    api_fail_hard();
}

int api_tick() {
    return _pycall_noargs(api.tick);
}



void api_drop() {
    Py_CLEAR(api.tick);
    Py_CLEAR(api.dispatch_mouseup);
    Py_CLEAR(api.dispatch_mousedown);
    Py_CLEAR(api.dispatch_mousemotion);
    Py_CLEAR(api.dispatch_key);
    Py_CLEAR(api.finalize);
    Py_CLEAR(api.__module);
}

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
        return API_FAILURE;
    return API_NOFAILURE;
}


#define PY_NOARGS 0
int _pycall_noargs(PyObject* func) {
    if(func && PyCallable_Check( func )) {
        PyObject_CallObject(func,PY_NOARGS);
        return api_checkfailure();
    } 
    return API_FAILURE;
}

int api_checkfailure_cleanargs(PyObject* args,PyObject** vals) {
    int i;
    if(PyErr_Occurred())
        return API_FAILURE;
    Py_CLEAR(args);
    return API_NOFAILURE;
}

#define MAX_ARGS 16
int _pycall_int_args(PyObject* func,int* args,int nargs) {
    int i;
    if(func && PyCallable_Check( func )) {
        PyObject* py_args = PyTuple_New(nargs);
        PyObject* py_vals [MAX_ARGS] = {0};

        for(i=0;i<nargs;++i) {
            py_vals[i] = PyInt_FromLong(args[i]);
            PyTuple_SetItem(py_args,i,py_vals[i]);
        }
        PyObject_CallObject(func,py_args);
        return api_checkfailure_cleanargs(py_args,py_vals);
    } 
    return API_FAILURE;
}
#undef MAX_ARGS
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

void api_dispatch_key(int key, int mode) {
    int args[2] = { key, mode };
    _pycall_int_args(api.dispatch_key, &args, 2);
}

void api_dispatch_mouseup(int button, int x, int y) {
    int args[3] = { button, x, y };
    _pycall_int_args(api.dispatch_mouseup, &args, 3);
}

void api_dispatch_mousedown(int button, int x, int y) {
    int args[3] = { button, x, y };
    _pycall_int_args(api.dispatch_mousedown, &args, 3);
}

void api_dispatch_mousemotion(int x, int y) {
    int args[2] = { x, y };
    _pycall_int_args(api.dispatch_mousemotion, &args, 2);
}

void api_drop() {
    _pycall_noargs(api.finalize);

    Py_CLEAR(api.tick);
    Py_CLEAR(api.dispatch_mouseup);
    Py_CLEAR(api.dispatch_mousedown);
    Py_CLEAR(api.dispatch_mousemotion);
    Py_CLEAR(api.dispatch_key);
    Py_CLEAR(api.finalize);
    Py_CLEAR(api.__module);
}

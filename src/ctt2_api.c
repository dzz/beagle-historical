#undef _DEBUG
#include <Python.h>

#include "system/ctt2.h"
#include "ctt2_api.h"

char* main_py;

typedef struct {
    PyObject* __module;
    PyObject* tick;
    PyObject* dispatch_mouseup;
    PyObject* dispatch_mousedown;
    PyObject* dispatch_mousemotion;
    PyObject* dispatch_key;
    PyObject* finalize;
} API_FUNCS;

static API_FUNCS api;


void api_init() {

    PyObject *pName, *pModule, *pDict;
    pName = PyString_FromString("py-scr.main");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if(pModule == 0) goto pyfailure;
    api.__module = pModule;
    
    #define LOAD_PY(x,y) api.##x = PyObject_GetAttrString(api.__module,y); if(api.##x==0) goto pyfailure;
    LOAD_PY(tick,"tick")
    LOAD_PY(dispatch_mouseup,"dispatch_mouseup");
    LOAD_PY(dispatch_mousedown,"dispatch_mousedown");
    LOAD_PY(dispatch_mousemotion,"dispatch_mousemotion");
    LOAD_PY(dispatch_key,"dispatch_key");
    LOAD_PY(finalize,"finalize");
    #undef LOAD_PY

    return;

pyfailure:
            if(PyErr_Occurred())
                PyErr_Print();
            printf("press a key...\n");
            getch();
            exit(1);
}

void api_tick() {
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

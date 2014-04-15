#undef _DEBUG
#include <stdio.h>
#include <conio.h>
#include <Python.h>

#include "system/ctt2.h"
#include "hwgfx/label.h"
#include "hwgfx/quads.h"
#include "ctt2_api.h"

#define MODULE_FUNC static PyObject* 
#define DEF_ARGS (PyObject *self, PyObject *args )
#define INPUT_ARGS PyArg_ParseTuple


typedef struct {
    PyObject* __module;
    #define CLIENT_FUNCTION(x,y) PyObject* x;
    #include "api-includes/client-handler-inventory.h"
    #undef CLIENT_FUNCTION
} CLIENT_HANDLERS;

static CLIENT_HANDLERS client_if;

void api_init_embedded_modules();

#include "api-includes\api_py_util.h"

int api_init() {
    PyObject *pName, *pModule, *pDict;
    api_init_embedded_modules();
    pName = PyString_FromString("kittens.main");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if(pModule == 0) goto pyfailure;
    client_if.__module = pModule;
    #define CLIENT_FUNCTION(x,y) client_if.##x = PyObject_GetAttrString(client_if.__module,y); if(client_if.##x==0) goto pyfailure;
    #include "api-includes/client-handler-inventory.h"
    #undef CLIENT_FUNCTION
    return _pycall_noargs(client_if.init);
pyfailure:
    api_fail_hard();
}

void api_drop() {
    _pycall_noargs(client_if.finalize);
    #define CLIENT_FUNCTION(x,y) Py_CLEAR(client_if.##x);
    #include "api-includes/client-handler-inventory.h"
    #undef CLIENT_FUNCTION
    Py_CLEAR(client_if.__module);
}

#include "api-includes/client-handlers.h"
#include "api-includes/modules/hwgfx.h"

void api_init_embedded_modules() {
    Py_InitModule("hwgfx", hwgfx_methods); 
    api_checkfailure();
}

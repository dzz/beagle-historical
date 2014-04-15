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
            py_vals[i] = PyInt_FromLong((long)args[i]);
            PyTuple_SetItem(py_args,i,py_vals[i]);
        }
        PyObject_CallObject(func,py_args);
        return api_checkfailure_cleanargs(py_args,py_vals);
    } 
    return API_FAILURE;
}

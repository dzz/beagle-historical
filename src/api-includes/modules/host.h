/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
 * import host
 * =============
 */

MODULE_FUNC host_abort
DEF_ARGS {
    exit(1);    
}

/*~=`=`=`=`=`=`=`=`=`=`==`=`=`=`=`=`=`=`=`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`=*/

static PyMethodDef host_methods[] = {
    {"abort",      host_abort,      METH_VARARGS, NULL},

    {NULL,NULL,0,NULL } /*terminator record*/
};

static PyModuleDef host_module = {
    PyModuleDef_HEAD_INIT, "host", NULL, -1, host_methods,
    NULL,NULL,NULL,NULL
};

static PyObject* PyInit_host() {
    return PyModule_Create(&host_module);
}

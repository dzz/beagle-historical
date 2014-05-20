/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
 * import host
 * =============
 */

MODULE_FUNC host_abort
DEF_ARGS {
    exit(1);    
}

MODULE_FUNC host_get_char_dims
DEF_ARGS {
    return Py_BuildValue("i", api_host_get_char_dims() );
}

/*~=`=`=`=`=`=`=`=`=`=`==`=`=`=`=`=`=`=`=`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`=*/

static PyMethodDef host_methods[] = {
    {"abort",               host_abort,                 METH_VARARGS, NULL},
    {"get_char_dims",       host_get_char_dims,         METH_VARARGS, NULL},
    {NULL,NULL,0,NULL } /*terminator record*/
};

static PyModuleDef host_module = {
    PyModuleDef_HEAD_INIT, "host", NULL, -1, host_methods,
    NULL,NULL,NULL,NULL
};

static PyObject* PyInit_host() {
    return PyModule_Create(&host_module);
}



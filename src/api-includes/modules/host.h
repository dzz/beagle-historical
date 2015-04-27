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

MODULE_FUNC host_get_gamepad_count
DEF_ARGS {
    return Py_BuildValue("i",SDL_NumJoysticks());
}

MODULE_FUNC host_dequeue_gamepad_dirty
DEF_ARGS {
    return Py_BuildValue("i",GamepadDequeueIsDirty() );
}

MODULE_FUNC host_get_hf_timer
DEF_ARGS {
    return Py_BuildValue("f",getTimeMs() );
}


MODULE_FUNC host_get_gamepad_sticks
DEF_ARGS {
    int i;
	hw_gamepad* gp;

    if(!INPUT_ARGS(args,"i",&i))
        return NULL;

    if(i>=MAX_PADS) 
        return NULL;

     gp = getGamepad(i);

    return Py_BuildValue("ffffff",    gp->left_x,
                                      gp->left_y,
                                      gp->right_x,
                                      gp->right_y,
                                      gp->left_trigger,
                                      gp->right_trigger );
}
/*~=`=`=`=`=`=`=`=`=`=`==`=`=`=`=`=`=`=`=`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`=*/

static PyMethodDef host_methods[] = {
    {"abort",               host_abort,                 METH_VARARGS, NULL},
    {"get_char_dims",       host_get_char_dims,         METH_VARARGS, NULL},
    {"get_gamepad_sticks",  host_get_gamepad_sticks,    METH_VARARGS, NULL},
    {"get_gamepad_count",   host_get_gamepad_count,     METH_VARARGS, NULL},
    {"dequeue_gamepad_dirty",   
                            host_dequeue_gamepad_dirty, METH_VARARGS, NULL},
    {"get_hf_timer",        host_get_hf_timer,          METH_VARARGS, NULL},

    {NULL,NULL,0,NULL } /*terminator record*/
};

static PyModuleDef host_module = {
    PyModuleDef_HEAD_INIT, "host", NULL, -1, host_methods,
    NULL,NULL,NULL,NULL
};

static PyObject* PyInit_host() {
    return PyModule_Create(&host_module);
}



/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
 * import localgfx
 * =============
 */

MODULE_FUNC localgfx_img_create
DEF_ARGS {
    int w,h;
    DRAWING_SURFACE ds;
    if(!INPUT_ARGS(args,"ii",&w,&h)) 
       return  NULL;

    ds = createDrawingSurface(w,h);
    return Py_BuildValue("I",(unsigned int)ds);
}

MODULE_FUNC localgfx_img_load
DEF_ARGS {
    char* py_str_txt;
    DRAWING_SURFACE ds;
    if(!INPUT_ARGS(args,"s", &py_str_txt)) 
        return NULL;
    ds = (DRAWING_SURFACE)IMG_Load(py_str_txt);
    return Py_BuildValue("I",(unsigned int)ds);
}

MODULE_FUNC localgfx_img_drop
DEF_ARGS {
    unsigned int ptr;
    if(!INPUT_ARGS(args,"I",&ptr))
        return NULL;
    destroyDrawingSurface((DRAWING_SURFACE)ptr);  
    Py_RETURN_NONE;
}

MODULE_FUNC localgfx_img_dims
DEF_ARGS {
    unsigned int ptr;
    SDL_Surface* surf;

    if(!INPUT_ARGS(args,"I",&ptr))
        return NULL;
    surf = (SDL_Surface*)ptr;
    return Py_BuildValue("ii",surf->w, surf->h);
}

/*~=`=`=`=`=`=`=`=`=`=`==`=`=`=`=`=`=`=`=`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`=*/
static PyMethodDef localgfx_methods[] = {
    {"localgfx_img_create",   localgfx_img_create,  METH_VARARGS, NULL},
    {"localgfx_img_load",     localgfx_img_load,    METH_VARARGS, NULL},
    {"localgfx_img_drop",     localgfx_img_drop,    METH_VARARGS, NULL},
    {"localgfx_img_dims",     localgfx_img_dims,    METH_VARARGS, NULL},

    {NULL,NULL,0,NULL } /*terminator record*/
};

static PyModuleDef localgfx_module = {
    PyModuleDef_HEAD_INIT, "localgfx", NULL, -1, localgfx_methods,
    NULL,NULL,NULL,NULL
};

static PyObject* PyInit_localgfx() {
    return PyModule_Create(&localgfx_module);
}

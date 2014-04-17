/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
 * import hwgfx
 * =============
 */

MODULE_FUNC hwgfx_draw_solidsquad 
DEF_ARGS  {
    int x,y,w,h;
    double r,g,b;
    if(!INPUT_ARGS(args,"iiiiddd",&x,&y,&w,&h,&r,&g,&b)) 
        return NULL;
    draw_solidquad(x,y,w,h,r,g,b);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_debug_displaykill
DEF_ARGS {
    DIRTY_DISPLAY_ABORT();
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_label_generate 
DEF_ARGS {
    gfx_label* label = malloc(sizeof(gfx_label));
    label_generate(label);
    { 
        unsigned int ptr = (unsigned int)label;
        printf("ptr out: %d",ptr);
    }
    return Py_BuildValue("I",(unsigned int)label);
}

MODULE_FUNC hwgfx_label_drop 
DEF_ARGS {
    unsigned int ptr; 
    gfx_label* label;
    if(!INPUT_ARGS(args,"I",&ptr)) 
        return NULL;
    label = (gfx_label*)ptr;
    label_drop (label);
    free(label);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_label_set_text 
DEF_ARGS {
    unsigned int ptr; 
    char *text; 
    gfx_label* label;
    char* py_str_txt;
    if(!INPUT_ARGS(args,"Is",&ptr, &py_str_txt)) 
        return NULL;
    label = (gfx_label*)ptr;
    text= py_str_txt;
    label_set_text(label,text);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_label_render
DEF_ARGS {
    Py_RETURN_NONE;
}


/*~=`=`=`=`=`=`=`=`=`=`==`=`=`=`=`=`=`=`=`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`=*/

static PyMethodDef hwgfx_methods[] = {
    {"draw_solidquad",      hwgfx_draw_solidsquad,      METH_VARARGS, NULL},
    {"debug_displaykill",   hwgfx_debug_displaykill,    METH_VARARGS, NULL},
    {"label_generate",      hwgfx_label_generate,       METH_VARARGS, NULL},
    {"label_drop",          hwgfx_label_drop,           METH_VARARGS, NULL},
    {"label_set_text",      hwgfx_label_set_text,       METH_VARARGS, NULL},

    {NULL,NULL,0,NULL } /*terminator record*/
};

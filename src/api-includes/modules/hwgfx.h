/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
 * import hwgfx
 * =============
 */


/**
 *  rect
 */
MODULE_FUNC hwgfx_rect_draw
DEF_ARGS  {
    int x,y,w,h;
    double r,g,b,a;

    if(!INPUT_ARGS(args,"iiiiI",&x,&y,&w,&h)) 
        return NULL;
    rect_draw(x,y,w,h);
    Py_RETURN_NONE;
}


/**
 * label
 */
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
    label   = (gfx_label*)ptr;
    text    = py_str_txt;
    label_set_text(label,text);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_label_render
DEF_ARGS {
    unsigned int ptr; 
    float x,y,r,g,b;
    gfx_label* label;
    if(!INPUT_ARGS(args,"Ifffff",&ptr,&x,&y,&r,&g,&b)) 
        return NULL;
    label = (gfx_label*)ptr;

    label_render (label, x, y,r,g,b);
    Py_RETURN_NONE;
}
/**
 * blend
 */
MODULE_FUNC hwgfx_blend_enter
DEF_ARGS{
    int mode;
    if(!INPUT_ARGS(args,"i",&mode))
        return NULL;
    blend_enter(mode);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_blend_exit
DEF_ARGS{
    blend_exit();
    Py_RETURN_NONE;
}

/**
 * shader
 */
MODULE_FUNC hwgfx_shader_load
DEF_ARGS {
    gfx_shader* shader = malloc(sizeof(gfx_shader));
    char*       vert;
    char*       frag;

    if(!INPUT_ARGS(args, "ss", &vert, &frag))
        return NULL;
    shader_load(shader, vert, frag);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_shader_drop
DEF_ARGS    {
    unsigned int ptr;

    if(!INPUT_ARGS(args,"I",&ptr))
        return NULL;
    shader_drop ((gfx_shader*)ptr);    
    free        ((gfx_shader*)ptr);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_shader_bind_float
DEF_ARGS {
    float x;
    unsigned int ptr;
    char* param;
    gfx_shader* shader;
    if(!INPUT_ARGS(args,"Isf",&ptr,&param,&x))
        return NULL;
    shader = (gfx_shader*)ptr;
    shader_bind_float(shader,param,x);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_shader_bind_vec2
DEF_ARGS {
    float x,y;
    unsigned int ptr;
    char* param;
    gfx_shader* shader;
    if(!INPUT_ARGS(args,"Isff",&ptr,&param,&x,&y))
        return NULL;
    shader = (gfx_shader*)ptr;
    shader_bind_vec2(shader,param,x,y);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_shader_bind_vec3
DEF_ARGS {
    float x,y,z;
    unsigned int ptr;
    char* param;
    gfx_shader* shader;
    if(!INPUT_ARGS(args,"Isfff",&ptr,&param,&x,&y,&z))
        return NULL;
    shader = (gfx_shader*)ptr;
    shader_bind_vec3(shader,param,x,y,z);
    Py_RETURN_NONE;
}

MODULE_FUNC hwgfx_shader_bind_vec4
DEF_ARGS {
    float x,y,z,w;
    unsigned int ptr;
    char* param;
    gfx_shader* shader;
    if(!INPUT_ARGS(args,"Isffff",&ptr,&param,&x,&y,&z,&w))
        return NULL;
    shader = (gfx_shader*)ptr;
    shader_bind_vec4(shader,param,x,y,z,w);
    Py_RETURN_NONE;
}

/**
 * debug
 */
MODULE_FUNC hwgfx_debug_displaykill
DEF_ARGS {
    DIRTY_DISPLAY_ABORT();
    Py_RETURN_NONE;
}


/*~=`=`=`=`=`=`=`=`=`=`==`=`=`=`=`=`=`=`=`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`=*/

static PyMethodDef hwgfx_methods[] = {
    {"debug_displaykill",   hwgfx_debug_displaykill,    METH_VARARGS, NULL},
    {"rect_draw",           hwgfx_rect_draw,            METH_VARARGS, NULL},
    {"label_generate",      hwgfx_label_generate,       METH_VARARGS, NULL},
    {"label_drop",          hwgfx_label_drop,           METH_VARARGS, NULL},
    {"label_set_text",      hwgfx_label_set_text,       METH_VARARGS, NULL},
    {"label_render",        hwgfx_label_render,         METH_VARARGS, NULL},
    {"blend_enter" ,        hwgfx_blend_enter,          METH_VARARGS, NULL},
    {"blend_exit" ,         hwgfx_blend_exit,           METH_VARARGS, NULL},
    {"shader_load" ,        hwgfx_shader_load,          METH_VARARGS, NULL},
    {"shader_drop" ,        hwgfx_shader_drop,          METH_VARARGS, NULL},

    {NULL,NULL,0,NULL } /*terminator record*/
};

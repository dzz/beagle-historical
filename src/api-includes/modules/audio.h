/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
 * import audio
 * =============
 */

MODULE_FUNC audio_clip_create
DEF_ARGS {
    audio_clip* clip;
    char *filename;
    clip = malloc(sizeof(clip));
    if(!INPUT_ARGS(args,"s",&filename))
        return NULL;
    audio_create_clip(clip,filename);
    return Py_BuildValue("I",(unsigned int)clip);
}

MODULE_FUNC audio_clip_drop
DEF_ARGS {
    unsigned int ptr;
    audio_clip* clip;
    if(!INPUT_ARGS(args,"I",&ptr))
        return NULL;
    clip = (audio_clip*)ptr;
    audio_drop_clip(clip);
    free(clip);
    Py_RETURN_NONE;
}

MODULE_FUNC audio_track_create
DEF_ARGS {
    audio_track* track;
    track = malloc(sizeof(track));
    audio_create_track(track);
    return Py_BuildValue("I",(unsigned int)track);
}

MODULE_FUNC audio_track_drop
DEF_ARGS {
    unsigned int ptr;
    audio_track* track;
    if(!INPUT_ARGS(args,"I",&ptr))
        return NULL;
    track = (audio_track*)ptr;
    free(track);
    Py_RETURN_NONE;
}

MODULE_FUNC audio_track_reset_all
DEF_ARGS {
    audio_reset_tracks();
    Py_RETURN_NONE;
}

MODULE_FUNC audio_track_play_clip
DEF_ARGS {
    unsigned int tptr,cptr,loop;
    audio_track* track;
    audio_clip* clip;

    if(!INPUT_ARGS(args,"III",&tptr,&cptr, &loop))  {
        return NULL;
    }

    track = (audio_track*)tptr;
    clip = (audio_clip*)cptr;
    audio_play_clip_on_track(clip,track,loop);
    Py_RETURN_NONE;
}

MODULE_FUNC audio_track_set_volume
DEF_ARGS {
    unsigned int tptr;
    float volume;
    audio_track* track;
    if(!INPUT_ARGS(args,"If",&tptr,&volume)) 
        return NULL;

    track = (audio_track*)tptr;
    audio_set_volume_on_track(track,(double)volume);
    Py_RETURN_NONE;
}

MODULE_FUNC audio_track_set_pan
DEF_ARGS {
    unsigned int tptr;
    float pan;
    audio_track* track;
    if(!INPUT_ARGS(args,"If",&tptr,&pan)) 
        return NULL;

    track = (audio_track*)tptr;
    audio_set_track_panning(track,(double)pan);
    Py_RETURN_NONE;
}


/*~=`=`=`=`=`=`=`=`=`=`==`=`=`=`=`=`=`=`=`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`=*/
static PyMethodDef audio_methods[] = {
    {"clip_create",         audio_clip_create,      METH_VARARGS, NULL},
    {"clip_drop",           audio_clip_drop,        METH_VARARGS, NULL},
    {"track_create",        audio_track_create,     METH_VARARGS, NULL},
    {"track_drop",          audio_track_drop,     METH_VARARGS, NULL},
    {"track_reset_all",     audio_track_reset_all,  METH_VARARGS, NULL},
    {"track_play_clip",     audio_track_play_clip,  METH_VARARGS, NULL},
    {"track_set_volume",    audio_track_set_volume, METH_VARARGS, NULL},
    {"track_set_pan",       audio_track_set_pan,    METH_VARARGS, NULL},
    {NULL,NULL,0,NULL } /*terminator record*/
};

static PyModuleDef audio_module = {
    PyModuleDef_HEAD_INIT, "audio", NULL, -1, audio_methods,
    NULL,NULL,NULL,NULL
};

static PyObject* PyInit_audio() {
    return PyModule_Create(&audio_module);
}

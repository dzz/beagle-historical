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

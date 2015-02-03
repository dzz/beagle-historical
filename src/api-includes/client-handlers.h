/* C bindings for python functions the host
 * can call */

int api_render() {
    return _pycall_noargs(client_if.render);
}

int api_tick() {
    return _pycall_noargs(client_if.tick);
}

int api_dispatch_key(int key, int mode) {
    int args[2] = { key, mode };
    return _pycall_int_args(client_if.dispatch_key, (int *)&args, 2);
}

int api_dispatch_mouseup(int button, int x, int y) {
    int args[3] = { button, x, y };
    return _pycall_int_args(client_if.dispatch_mouseup, (int *)&args, 3);
}

int api_dispatch_mousedown(int button, int x, int y) {
    int args[3] = { button, x, y };
    return _pycall_int_args(client_if.dispatch_mousedown, (int *)&args, 3);
}

int api_dispatch_mousemotion(int x, int y) {
    int args[2] = { x, y };
    return _pycall_int_args(client_if.dispatch_mousemotion, (int *)&args, 2);
}

int api_dispatch_text(char* text) {
    return _pycall_str_arg( client_if.dispatch_text, text );
}

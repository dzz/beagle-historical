/* C bindings for python functions the host
 * can call */

int api_tick() {
    return _pycall_noargs(client_if.tick);
}

void api_dispatch_key(int key, int mode) {
    int args[2] = { key, mode };
    _pycall_int_args(client_if.dispatch_key, &args, 2);
}

void api_dispatch_mouseup(int button, int x, int y) {
    int args[3] = { button, x, y };
    _pycall_int_args(client_if.dispatch_mouseup, &args, 3);
}

void api_dispatch_mousedown(int button, int x, int y) {
    int args[3] = { button, x, y };
    _pycall_int_args(client_if.dispatch_mousedown, &args, 3);
}

void api_dispatch_mousemotion(int x, int y) {
    int args[2] = { x, y };
    _pycall_int_args(client_if.dispatch_mousemotion, &args, 2);
}

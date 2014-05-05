import host

from client.ui.mod_empty    import mod_empty
from client.ui.areas        import SIGNAL_EXIT_HANDLER
from client.ui.areas        import SIGNAL_CONTINUE_HANDLING
from client.ui.areas        import xy_in_r

class _mouse_origin:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.origins = [[0,0]]

    def get_origin(self):
        return [ self.x, self.y ]

    def push_origin(self,x,y):
        self.origins.append( [x,y] )
        self.x = x
        self.y = y

    def pop_origin(self):
        self.origins.pop()
        origin = self.origins[ len(self.origins)-1 ] 
        self.x = origin[0]
        self.y = origin[1]

_morg = _mouse_origin()

def get_mouse_origin():
    global _morg
    return _morg


class mod_parent(mod_empty):
    def rcv_mousemotion(self,ui_area,x,y):
        mo = get_mouse_origin()
        mo.push_origin(ui_area.r[0], ui_area.r[1]) 
        for child in ui_area.children:
            if(xy_in_r(x,y,child.r)):
                mo.push_origin(child.r[0],child.r[1]);
                origin = mo.get_origin()
                xt = x - origin[0];
                yt = y - origin[1] - 12;
                child.set_m([xt,yt])
                mo.pop_origin()

        mo.pop_origin()

    #def rcv_mouse_button(self,ui_area,button,x,y,down):
    #    return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [down])

    #def rcv_key(self,ui_area,key,down):
    #    return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [key,down])


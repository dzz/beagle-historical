import host

from client.ctt2.mouse_focus import capture_mouse
from client.ui.mod_empty    import mod_empty
from client.ui.areas        import SIGNAL_EXIT_HANDLER
from client.ui.areas        import SIGNAL_CONTINUE_HANDLING
from client.ui.areas        import xy_in_r

class mod_parent(mod_empty):

    def __init__(self):
        self.focused_area = None

    def rcv_mousemotion(self,ui_area,x,y):
        if(self.focused_area is not None):
            xt = x - self.focused_area.r[0];
            yt = y - self.focused_area.r[1];
            return self.focused_area.rcv_mousemotion(xt,yt)

        for child in ui_area.children:
            if(xy_in_r(x,y,child.r)):
                xt = x - child.r[0];
                yt = y - child.r[1];
                return child.rcv_mousemotion(xt,yt)


    def rcv_mouse_button(self,ui_area,button,x,y,down):
        if( down is False and self.focused_area is not None):
            xt = x - self.focused_area.r[0];
            yt = y - self.focused_area.r[1];
            rval = self.focused_area.rcv_mouse_button(button,xt,yt,down)
            self.focused_area = None
            return rval

        else:
            for child in ui_area.children:
                if(xy_in_r(x,y,child.r)):
                    xt = x - child.r[0];
                    yt = y - child.r[1];
                    self.focused_area = child
                    return child.rcv_mouse_button(button,xt,yt,down)


    #def rcv_mouse_button(self,ui_area,button,x,y,down):
    #    return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [down])

    #def rcv_key(self,ui_area,key,down):
    #    return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [key,down])


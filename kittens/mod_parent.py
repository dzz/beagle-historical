from ui_area import *
from mod_empty import *

def _cascade_signal(ui_area,func,x,y,addtl_params):
        for child in ui_area.children:
            if(xy_in_r(x,y,child.r)):
                    x_t = x - ui_area.x
                    y_t =y - ui_area.y
                    child.func(x_t,y_t,*addtl_params)
                    return SIGNAL_EXIT_HANDLER
        return SIGNAL_CONTINUE_HANDLING

class mod_parent(mod_empty):
    def rcv_mousemotion(self,ui_area,x,y):
        return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y,[])

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [down])

    #def rcv_key(self,ui_area,key,down):
    #    return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [key,down])

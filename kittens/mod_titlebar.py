from ui_area import *
from mod_empty import *

class mod_titlebar(mod_empty):
    def __init__(self,ui_area,titlebar_text,height):
        self.cursor_origin = [0,0]
        self.origin = [0,0]
        self.height = height
        self.toggled = False
        ui_area.prop["titlebar_text"] = titlebar_text
        ui_area.prop["titlebar_height"] = height

    def transform_client_area(self,r):
        r[3] -= self.height
        r[1] += self.height
        return r

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        if down == False:
            self.toggled = False
            return SIGNAL_CONTINUE_HANDLING 
        else:
            if( y < self.height ):
                self.toggled = True
                self.origin = [ ui_area.r[0], ui_area.r[1] ]
                mpos = glbl_mpos()
                self.cursor_origin = [ mpos[0], mpos[1] ]
                return SIGNAL_EXIT_HANDLER

    def rcv_mousemotion(self,ui_area,x,y):
        if self.toggled == False:
            return SIGNAL_CONTINUE_HANDLING
        else:
            mpos = glbl_mpos()
            ui_area.r[0] = self.origin[0] + mpos[0] - self.cursor_origin[0]
            ui_area.r[1] = self.origin[1] + mpos[1] - self.cursor_origin[1]
            self.move_origin = [x,y]
            return SIGNAL_EXIT_HANDLER

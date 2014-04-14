from ui_area import *

class mod_empty(object):
    def __init__(self):
        pass

    def rcv_mousemotion(self,ui_area,x,y):
        return SIGNAL_CONTINUE_HANDLING

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        return SIGNAL_CONTINUE_HANDLING

    def rcv_key(self,ui_area,key,down):
        return SIGNAL_CONTINUE_HANDLING

    def transform_client_area(self,r):
        return r

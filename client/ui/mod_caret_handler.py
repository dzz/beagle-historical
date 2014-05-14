import host
from client.ui.mod_empty    import mod_empty
import client.ctt2.caret    as caret


class mod_caret_handler(mod_empty):
    def rcv_mouse_button(self,ui_area,button,x,y,down):
        if(down):
            caret.acquire(ui_area)


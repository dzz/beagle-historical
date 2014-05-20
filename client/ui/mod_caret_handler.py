import host
from client.ui.mod_empty    import mod_empty
import client.ctt2.caret    as caret


class mod_caret_handler(mod_empty):
    def rcv_mouse_button(self,ui_area,button,x,y,down):
        if(down):
            caret.acquire(ui_area)
            ui_area.begin_edit()
            ui_area.set_text("")

    def rcv_key(self,ui_area,key,down):
        if(caret.get_caret() == ui_area):
            if key == 27:  #escape
                caret.drop()
                ui_area.revert_edit()
            if key == 8:    #backspace
                text = ui_area.get_text()
                if(len(text)>0):
                    text = text [0:len(text)-1]
                    ui_area.set_text(text)
            if key == 13:   #CR
                caret.drop()
                ui_area.end_edit()


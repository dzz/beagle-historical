import host
from client.gfx.label               import label

_status_str = None

status_label = label("")
status_label2 = label("")

status_col = None

status_col_go   = [0.0,1.0,0.0]
status_col_stop = [1.0,0.0,0.0]


#i.d.g.a.f -    path of least resistance
#               for basic fb in ui 

def _set_text(text):
    text_len= 19
    add = "";
    if(len(text)>text_len):
        add = ""
    status_label.set_text(text[0:text_len]+add)
    status_label2.set_text(text[text_len:len(text)])

def set_status(text):
    global status_col
    status_col = status_col_go
    _set_text(text)

def set_error(text):
    global status_col
    status_col = status_col_stop
    _set_text(text)

def render_status():
    status_label.draw(0,0,status_col)
    status_label2.draw(0,host.get_char_dims() ,status_col)

